% Hanna Johansson ImSecu 2017-06-09
% Lab 2: Face eecognition using eigenfaces
% Exercise 4: Mismatch between the eigenspace and test individuals 

close all;
%clear all;

% Add path to the public directory to access ready-made functions
addpath T:\courses\image\TpBiometry\public\Matlab

load('Means');
load('Space');

disp('-----------------------------------------------');
disp('  PART A: Computing ident. rates for set B ');
disp('-----------------------------------------------');

% Load the images from the training set, train_B
fprintf('--> Loading training images.. \n')
path_trainB = '\\datas\teaching\courses\image\TpBiometry\public\Images\train_B\';
trainB_images = loadImagesInDirectory(path_trainB);
fprintf('--> Train B have been loaded \n')

% Load the images from the test set, test_B
fprintf('--> Loading test images.. \n')
path_testB = '\\datas\teaching\courses\image\TpBiometry\public\Images\test_B\';
testB_images = loadImagesInDirectory(path_testB);
fprintf('--> Test B have been loaded \n\n')

% Enrolling (projecting) training images of Train B into Space A 
trainB_locations = projectImages(trainB_images, Means, Space);
% Enrolling (projecting) training images of Train B into Space A 
testB_locations = projectImages(testB_images, Means, Space);
fprintf('--> Train B and Test B have been projected into Space A\n')

% Plot the coordinates of the 5 test faces of the first 5 test individuals 
% in Test B in the space spanned by the first 3 eigenfaces in Space A
projImg3 = plotFirst3Coordinates(testB_locations, 5, 5);
title('Projected images from Test B in Space A');
saveas(projImg3,'images\projImg3','jpg');
fprintf('--> Projected image is saved \n\n')

% Compute the identification rate for a varying number of eigenfaces
fprintf('--> Calculating identification rates.. \n')
for i=1:10:100
    IdentificationRate_part4A = identify(trainB_locations, testB_locations, i,1);
    fprintf('Nmbr of eigenfaces = %d\t', i);
    fprintf('--> identification rate: %d\n', IdentificationRate_part4A);
end
fprintf('--> Identification rates computed \n\n');

% OPTIONAL --> Plotting the identification rates 
nmbr_Eigenfaces = zeros(length(50));
ident_Rate2 = zeros(length(50));
max_IR2 = 0;
bestnmbr_Eigen3 = 0;
for i=1:100
    IdentificationRate_part4A = identify(trainB_locations, testB_locations, i,1);
    nmbr_Eigenfaces(i) = i;
    ident_Rate2(i) = IdentificationRate_part4A;
    
    % Find best number of eigenfaces for maximum identification rate
    max_IR2 = max(ident_Rate2);
    if ident_Rate2(i) == max(ident_Rate2)
    	bestnmbr_Eigen3 = nmbr_Eigenfaces(i);
    end
end
fprintf('--> Best identification rate = %d', max_IR2);
fprintf(', when using %d', bestnmbr_Eigen3);
fprintf(' eigenfaces \n\n');

% Plot the identification rate as a function of the number of eigenfaces
figure;
identRate_fig3 = plot(nmbr_Eigenfaces, ident_Rate2, '-');
% % Plot the maximum identification rate
% hold on
% plot(bestnmbr_Eigen, max_IR, 'or');
title('The identification rate as a function of the number of eigenfaces');
fprintf('--> Identification rates plotted \n\n');
xlabel('Eigenfaces');
ylabel('Identification rate');

% Save the figure
saveas(identRate_fig3, 'images\identRate_fig3', 'jpg');
fprintf('--> Figure saved \n\n');

%% PART B: Computing the eigenspace B
disp('-----------------------------------------------');
disp('  PART B: Computing the eigenspace B');
disp('-----------------------------------------------');

% Build the space, space_B
[Means_B, Space_B, Eigenvalues_B] = buildSpace(trainB_images);
fprintf('--> Eigenspace B is built \n')

% Save the space
save 'space_B' Space_B;
fprintf('--> Eigenspace B is saved \n\n')

% Enrolling (projecting) training images of Train B into Space B 
trainB_locationsB = projectImages(trainB_images, Means_B, Space_B);
% Enrolling (projecting) training images of Train B into Space B 
testB_locationsB = projectImages(testB_images, Means_B, Space_B);
fprintf('--> Train B and Test B have been projected into Space B\n')

% Plot the coordinates of the 5 test faces of the first 5 test individuals 
% in Test B in the space spanned by the first 3 eigenfaces in Space B
projImg4 = plotFirst3Coordinates(testB_locationsB, 5, 5);
title('Projected images from Test B in Space B');
saveas(projImg4,'images\projImg4', 'jpg');
fprintf('--> Projected image is saved \n\n')

% Compute the identification rate for a varying number of eigenfaces
fprintf('--> Calculating identification rates.. \n')
for i=1:10:100
    IdentificationRate_part4B = identify(trainB_locationsB, testB_locationsB, i,1);
    fprintf('Nmbr of eigenfaces = %d\t', i);
    fprintf('--> identification rate: %d\n', IdentificationRate_part4B);
end
fprintf('--> Identification rates computed \n\n');

% OPTIONAL --> Plotting the identification rates 
nmbr_Eigenfaces = zeros(length(50));
ident_Rate2 = zeros(length(50));
max_IR2 = 0;
bestnmbr_Eigen4 = 0;
for i=1:100
    IdentificationRate_part4B = identify(trainB_locationsB, testB_locationsB, i,1);
    nmbr_Eigenfaces(i) = i;
    ident_Rate2(i) = IdentificationRate_part4B;
    
    % Find best number of eigenfaces for maximum identification rate
    max_IR2 = max(ident_Rate2);
    if ident_Rate2(i) == max(ident_Rate2)
    	bestnmbr_Eigen4 = nmbr_Eigenfaces(i);
    end
end
fprintf('--> Best identification rate = %d', max_IR2);
fprintf(', when using %d', bestnmbr_Eigen4);
fprintf(' eigenfaces \n\n');

% Plot the identification rate as a function of the number of eigenfaces
figure;
identRate_fig4 = plot(nmbr_Eigenfaces, ident_Rate2, '-');
% % Plot the maximum identification rate
% hold on
% plot(bestnmbr_Eigen, max_IR, 'or');
title('The identification rate as a function of the number of eigenfaces');
fprintf('--> Identification rates plotted \n\n');
xlabel('Eigenfaces');
ylabel('Identification rate');

% Save the figure
saveas(identRate_fig4, 'images\identRate_fig4', 'jpg');
fprintf('--> Figure saved \n\n');

disp('-----------------------------------------------');
disp('                END OF SCRIPT');
disp('-----------------------------------------------');
























