% Hanna Johansson ImSecu HOMEWORK 2017-06-09

close all;
%clear all;

% Add path to the SPNandESPNextractor
addpath T:\courses\image\TpSensorIdentification\public\MATLAB
% Add path to my personal dataset
myDir ='\\datas\teaching\courses\ImSecu\challenge\johanssh';

% EXERCISE 1: The Reference Sensor Pattern Noise
% RSPN = The "fingerprint template" of a certain camera

% PART A: Compute the RSPN from background photos
% Extract the 50 uniform background pictures from my database
D_BG = dir(strcat(myDir,'\Eurecom_*_picBG_*.jpg'));

% Declare empty array for the SPN values
spn = [];

disp('-------------------------------------------');
disp('     Compute SPN for all BG pictures');
disp('-------------------------------------------');
% Use the function 'SPNandESPNextractor' to extract the SPN of each picture
for i=1:length(D_BG)
    % Extract file name from database
    image_name = D_BG(i).name;
    % Path to file
    image_path = fullfile(myDir, image_name);
    fprintf(1, 'Reading file: %s\n', image_name);
    % Import the image
    image = importdata(image_path);
    % Extract SPN, ESPN and S from the image
    [SPN, ESPN, S] = SPNandESPNextractor(image);
    spn = [spn; SPN];
end
disp('--> Computation completed for all BG pictures');

% Compute the average SPN over the N/2 pictures (i.e. the RSPN)
averageSPN = mean(spn);
fprintf('--> Average SPN computed\n\n')

% Save the obtained RSPN according to the naming convention: Eurecom_***__rspnBG.mat
save 'Eurecom_174_rspnBG' averageSPN;


%% EXERCISE 2: Computing SPN and ESPN
% SPN = The 'fingerprint' left by a sensor on a single photo
% ESPN = The enhanced version of the SPN

% Extract the 50 uniform foreground pictures from my database
D_FG = dir(strcat('T:\courses\ImSecu\challenge\johanssh', '\Eurecom_*_picFG_*.jpg'));

% PART A: Compare the SPN and the ESPN of a photo
% Add path to the picture 'Eurecom_202_picFG_040.jpg'
imageDir = '\\datas\teaching\courses\image\TpSensorIdentification\public\Images';
picFG = dir(strcat(imageDir,'\Eurecom_202_picFG_040.jpg'));

disp('-------------------------------------------');
disp('     Extract SPN and ESPN');
disp('-------------------------------------------');
% Use the 'SPNandESPNextractor' to compute the SPN and ESPN of the picture
image_name = picFG.name;
% Path to file
image_path = fullfile(imageDir, image_name);
fprintf(1, 'Reading file: %s\n', image_name);
% Import the image
image = importdata(image_path);
% Extract SPN, ESPN and S from the image
[SPN_partA, ESPN_partA, S_partA] = SPNandESPNextractor(image);
fprintf('--> SPN and ESPN extracted\n\n');

% Use the matlab function 'waverec2' to obtain two images representing the 
% SPN and ESPN extracted from the central part of the orignal photo, respectively.
% Inputs: SPN/ESPN, the bookkeeping matrix S, 'db8' (wavelet indication)
SPN_image = waverec2(SPN_partA, S_partA, 'db8');
ESPN_image = waverec2(ESPN_partA, S_partA, 'db8');

% Visualize and compare the two images
SPN_fig = imshow(SPN_image);
figure;
ESPN_fig = imshow(ESPN_image);
figure;

hist_montage = imshowpair(SPN_image, ESPN_image,'montage');
title('SPN image to the left,             ESPN image to the right' );

% Save images
saveas(hist_montage,'hist_montage', 'jpg');
saveas(SPN_fig, 'SPN_image', 'jpg');
saveas(ESPN_fig, 'ESPN_image', 'jpg');

%% PART B: Computing the EPSN of each photo

% I=5 random pictures, with concealed file names, for testing
addpath T:\courses\image\TpSensorIdentification\public\Test_images
% J RSPNs that are the RSPNs computed by classmates
addpath T:\courses\image\TpSensorIdentification\public\Test_RSPNs
% The path to the directory
publicDir = '\\datas\teaching\courses\image\TpSensorIdentification\public';

% Import test images and test RSPNs
test_images = dir(strcat(publicDir,'\Test_images\*.jpg'));
% Import all the RSPNs
fprintf('--> Load all RSPNs\n');
load('allRSPNs');
fprintf('--> Loading completed\n\n');

% Use the 'SPNandESPNextractor' to extract the ESPN for each of the I pictures
% Declare empty array for the ESPN values
espn_list = [];

disp('-------------------------------------------');
disp('     Compute ESPN for all test images');
disp('-------------------------------------------');
fprintf('--> Read and compute EPSN for each file\n');
% Use the function 'SPNandESPNextractor' to extract the SPN of each picture
for j=1:length(test_images)
    % Extract file name from database
    image_name = test_images(j).name;

    fprintf(1, 'Reading file: %s\n', image_name);
    % Read the image
    image = imread(image_name);

    % Extract SPN, ESPN and S from the image
    [SPN, ESPN, S] = SPNandESPNextractor(image);
    espn_list = [espn_list; ESPN];
end
fprintf('--> Computation completed for all images\n\n');

disp('-------------------------------------------');
disp('     Compute the correlation score');
disp('-------------------------------------------');
% Compute the correlaction score of the ESPN with each RSPN

% Build a Distance Matrix I*J. Rows = the I ESPNs, columns = the J RSPNs
% In each cell c(i,j) put the correlation score
% The function 'who': list all variable names that are in the workspace
% The function 'eval': possible to use the variable and not only its name
rspn_name_list = who('-regexp', 'Eurecom_*');
K = 5;  
N = size(rspn_name_list, 1);
corr_matrix = [length(K);length(N)];
fprintf('--> Build a Distance Matrix\n');
corr_scores = zeros(length(test_images),1);
sensor = zeros(5,1);
for i = 1:K
    for j = 1:N
        [corr_matrix(i,j), lags] = xcorr(eval(rspn_name_list{j}), espn_list(i,:),0,'coeff');
    
        % Check for maximum correlation score for each image
        max_corr = max(corr_matrix(i,:));
        if corr_matrix(i,j) == max_corr
            corr_scores(i,1) = max_corr;
            sensor(i,1) = j;
        end

    end
end
fprintf('--> Distance Matrix built\n');

fprintf('--> Displaying Distance Matrix\n\n');
imagesc(corr_matrix);
colormap('jet');
colorbar;

% Save the Distance Matrix
save 'corr_matrix' corr_matrix;

disp('-------------------------------------------');
disp('     RESULTS:');
disp('-------------------------------------------');
fprintf('Picture \t\t Sensor used \t\t Correlation score: \n');
for k=1:length(sensor)
    fprintf('%s', test_images(k).name);
    fprintf('\t %s', rspn_name_list{sensor(k)});
    fprintf('\t %.8f \n', corr_scores(k,1));
end

disp('-----------------------------------------------');
disp('                END OF SCRIPT');
disp('-----------------------------------------------');















