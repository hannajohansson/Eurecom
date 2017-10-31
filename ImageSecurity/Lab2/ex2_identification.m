% Hanna Johansson ImSecu 2017-06-09
% Lab 2: Face eecognition using eigenfaces
% Exercise 2: Identification

% Obtained: our first identification & cumulative identification results
% Question: "Which are the N individuals associated with the best ranks?"

close all;
%clear all;

% Add path to the public directory to access ready-made functions
addpath T:\courses\image\TpBiometry\public\Matlab

disp('-----------------------------------------------');
disp('  PART A: Projecting and plotting Test A ');
disp('-----------------------------------------------');
% PART A: Projecting and plotting Test A 

% Load the images from the test set, test_A
fprintf('--> Loading images.. \n')
path = '\\datas\teaching\courses\image\TpBiometry\public\Images\test_A\';
test_images = loadImagesInDirectory(path);
fprintf('--> Test images have been loaded \n')
save 'test_images' test_images;

% Project test images of Test A into Space A
fprintf('--> Loading images.. \n')
test_locations = projectImages(test_images, Means, Space);
fprintf('--> Test images projected into Space A \n')
save 'test_locations' test_locations;

% Draw the location of the 5 test points of the first 5 individuals
% in the space spanned by the first 3 eigenvectors of Space A.
projImg2 = plotFirst3Coordinates(test_locations, 5, 5);
title('Projected test images in Space A');

% Save the images
saveas(projImg2,'images\projImg2','jpg');
fprintf('--> Projected image is saved \n\n')

%% PART B: Approximating s1_6.jpg
% Try to rebuild the first test face of the first person with varying
% number of eigenfaces
disp('-----------------------------------------------');
disp('  PART B: Approximating s1_6.jpg');
disp('-----------------------------------------------');

% Path to file
filePath = strcat(path,'s1_6.jpg');

% FigureHandle = approximateImage(FileName, Means, Space, Threshold)
figHandle1 = approximateImage(filePath, Means, Space, 6);
figHandle2 = approximateImage(filePath, Means, Space, 15);
figHandle3 = approximateImage(filePath, Means, Space, 30);
figHandle4 = approximateImage(filePath, Means, Space, 50);
figHandle5 = approximateImage(filePath, Means, Space, 100);

% Save the images
saveas(figHandle1,'images\approx_TestImg1','jpg');
saveas(figHandle2,'images\approx_TestImg2','jpg');
saveas(figHandle3,'images\approx_TestImg3','jpg');
saveas(figHandle4,'images\approx_TestImg4','jpg');
saveas(figHandle5,'images\approx_TestImg5','jpg');

fprintf('--> Approximated images saved and displayed \n\n')

%% PART C: Computing the identification rates (first face)
% Compute the identification rates by comparing the test identities (the
% projected images of Test A in Space A) against the enrolled identities
% (the projected images of Train A in Space A).
disp('-----------------------------------------------');
disp('  PART C: Computing the identification rates');
disp('-----------------------------------------------');

% Project the image vectors into Space A
test_locations = projectImages(test_images, Means, Space);
save 'test_locations' test_locations;
% Since we want as reference point the first training face of each
% individual, these have to be extracted
location_first_faces = train_locations(1:5:end,:);
save 'location_first_faces' location_first_faces;

% Compute the identification rate for a varying number of eigenfaces
%   'Locations' contains the coordinates of the faces of various individuals
%   'Models' contains the coords of the training images = train_locations
%   'Test' contains the coords of the test images = test_locations

% IdentificationRate = identify(Models, Test, Threshold, NBest)
IdentificationRate = identify(location_first_faces, test_locations, 10,1);

% Compute the identification rate for different number of eigenfaces
for i=1:20:100
    IdentificationRate = identify(location_first_faces, test_locations, i,1);
    fprintf('Nmbr of eigenfaces = %d\t', i);
    fprintf('--> identification rate: %d\n', IdentificationRate);
end

% Compute the identification rate for all 100 eigenfaces, to be able to 
% plot the function later on
fprintf('--> Computing identification rates \n');
nmbr_Eigenfaces = zeros(length(50));
ident_Rate = zeros(length(50));
max_IR = 0;
bestnmbr_Eigen = 0;
for i=1:100
    IdentificationRate = identify(location_first_faces, test_locations, i,1);
    nmbr_Eigenfaces(i) = i;
    ident_Rate(i) = IdentificationRate;
    
    % Find best number of eigenfaces for maximum identification rate
    max_IR = max(ident_Rate);
    if ident_Rate(i) == max(ident_Rate)
    	bestnmbr_Eigen = nmbr_Eigenfaces(i);
    end
end
fprintf('--> Identification rates computed \n\n');
fprintf('--> Best identification rate = %d', max_IR);
fprintf(', when using %d', bestnmbr_Eigen);
fprintf(' eigenfaces \n\n');

fprintf('Highest identification rates \n');
% Highest identification rate
for i=35:36
    IdentificationRate = identify(location_first_faces, test_locations, i,1);
    fprintf('Nmbr of eigenfaces = %d\t', i);
    fprintf('--> identification rate: %d\n', IdentificationRate);
end

% Plot the identification rate as a function of the number of eigenfaces
figure;
identRate_fig = plot(nmbr_Eigenfaces, ident_Rate, '-');
% % Plot the maximum identification rate
% hold on
% plot(bestnmbr_Eigen, max_IR, 'or');
title('The identification rate as a function of the number of eigenfaces');
fprintf('--> Identification rates plotted \n');
xlabel('Eigenfaces');
ylabel('Identification rate');

% Save the figure
saveas(identRate_fig, 'images\identRate_fig','jpg');
fprintf('--> Figure saved \n\n');

%% PART D: More identification rates (mean face)
% Repeat the previous question, using the average of the five training
% points as reference point of each individual 
disp('-----------------------------------------------');
disp('  PART D: More identification rates (mean face)');
disp('-----------------------------------------------');

% Calculate the mean of the five projected images for each individual
% We have 20 individuals
fprintf('--> Calculate mean for each individual \n');
projected_mean = zeros(20,100);
for i=1:20
    locations_first_five = train_locations((i-1)*5+1:i*5,:);
    mean_location = mean(locations_first_five);
    projected_mean(i,:) = mean_location;
end 
fprintf('--> Mean calculated \n\n');

% IdentificationRate = identify(Models, Test, Threshold, NBest)
fprintf('--> Computing identification rates \n');
for i=1:20:100
    IdentificationRate = identify(projected_mean, test_locations, i,1);
    fprintf('Nmbr of eigenfaces = %d\t', i);
    fprintf('--> identification rate: %d\n', IdentificationRate);
end

nmbr_Eigenfaces = zeros(length(50));
ident_Rate = zeros(length(50));
max_IR = 0;
bestnmbr_Eigen = 0;
for i=1:100
    IdentificationRate = identify(projected_mean, test_locations, i,1);
    nmbr_Eigenfaces(i) = i;
    ident_Rate(i) = IdentificationRate;
    
    % Find best number of eigenfaces for maximum identification rate
    max_IR = max(ident_Rate);
end
fprintf('--> Best identification rate = %d\n', max_IR);

% Plot the identification rate as a function of the number of eigenfaces
figure;
identRate_fig1 = plot(nmbr_Eigenfaces, ident_Rate, '-');
title('The identification rate as a function of the number of eigenfaces');
fprintf('--> Identification rates plotted \n\n');
xlabel('Eigenfaces');
ylabel('Identification rate');
saveas(identRate_fig1, 'images\identRate_fig1','jpg');

%% PART E: Drawing identification rates as a function of N-Best
disp('-----------------------------------------------');
disp('  PART E: Ident. rates as a function of N-Best');
disp('-----------------------------------------------');

% Reference points = the same as in part D
% Save the reference points for usage later on
save 'projected_mean' projected_mean;

% Project test images of Test A into Space A
test_locations = projectImages(test_images, Means, Space);
fprintf('--> Test images projected into Space A \n')
save 'test_locations' test_locations;

% For a fixed number of eigenfaces (e.g. 5) compute the N-Best
% identification rates, for various, N, using 'identify'.
% IdentificationRate = identify(projected_mean, test_locations, 5, 5);
fprintf('--> Computing identification rates \n\n');
for N=1:2:20
    IdentificationRate = identify(projected_mean, test_locations, 5, N);
    fprintf('N = %d\t', N);
    fprintf('--> identification rate: %d\n', IdentificationRate);
end

% Compute identification rates to plot
nmbr_N = zeros(length(50));
ident_Rate = zeros(length(50));
max_IR = 0;
bestnmbr_N = 0;
for i=1:20
    IdentificationRate = identify(projected_mean, test_locations,5,i);
    nmbr_N(i) = i;
    ident_Rate(i) = IdentificationRate;
    
    % Find best number of eigenfaces for maximum identification rate
    max_IR = max(ident_Rate);
    if ident_Rate(i) == max(ident_Rate)
    	bestnmbr_N = nmbr_N(i);
    end
end
fprintf('--> Best identification rate = %d\n', max_IR);

% Plot the identification rate as a function of the number of eigenfaces
figure;
identRate_fig2 = plot(nmbr_N, ident_Rate, '-');
% % Plot the maximum identification rate
% hold on
% plot(bestnmbr_Eigen, max_IR, 'or');
title('The identification rate as a function of parameter N');
fprintf('--> Identification rates plotted \n\n');
xlabel('Parameter N');
ylabel('Identification rate');
saveas(identRate_fig2, 'images\identRate_fig2','jpg');

disp('-----------------------------------------------');
disp('                END OF SCRIPT');
disp('-----------------------------------------------');








