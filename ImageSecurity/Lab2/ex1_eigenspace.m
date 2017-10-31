% Hanna Johansson ImSecu 2017-06-09
% Lab 2: Face eecognition using eigenfaces
% Exercise 1: Building the eigenspace

close all;
%clear all;

% Add path to the public directory to access ready-made functions
addpath T:\courses\image\TpBiometry\public\Matlab

% PART A: Computing the eigenspace A
% Use the set Train A to train the eigenspace that we will call Space A
disp('-----------------------------------------------');
disp('  PART A: Computing the eigenspace A ');
disp('-----------------------------------------------');

% Load the images from the training set, train_A
path = '\\datas\teaching\courses\image\TpBiometry\public\Images\train_A\';
images = loadImagesInDirectory(path);
fprintf('--> Training images have been loaded \n')

% Build the space, space_A
[Means, Space, Eigenvalues] = buildSpace(images);
fprintf('--> Eigenspace A is built \n')

% Save the space and variables
save 'Space' Space;
save 'Means' Means;
save 'Eigenvalues' Eigenvalues;
fprintf('--> Eigenspace A is saved \n\n')

%% PART B: Plotting the cumulative sum of eigenvalues
disp('-----------------------------------------------');
disp('  PART B: Plotting the cumulative sum');
disp('-----------------------------------------------');

cumulatedSum = cumsum(Eigenvalues);
cumulatedSum_fig = plot(cumulatedSum);
title('Cumulated sum of the eigenvalues');
fprintf('--> Cumulated sum displayed in plot \n\n')
saveas(cumulatedSum_fig, 'images\cumulatedSum_fig', 'jpg');
save 'cumulatedSum' cumulatedSum;

%% PART C: Approximating s1_1.jpg
disp('-----------------------------------------------');
disp('  PART C: Approximating s1_1.jpg');
disp('-----------------------------------------------');

% Path to file
filePath = strcat(path,'s1_1.jpg');

% FigureHandle = approximateImage(FileName, Means, Space, Threshold)
% Try with a varying number of eigenfaces
figureHandle1 = approximateImage(filePath, Means, Space, 6);
figureHandle2 = approximateImage(filePath, Means, Space, 15);
figureHandle3 = approximateImage(filePath, Means, Space, 30);
figureHandle4 = approximateImage(filePath, Means, Space, 50);
figureHandle5 = approximateImage(filePath, Means, Space, 100);

% Save the images
saveas(figureHandle1,'images\approxImg1','jpg');
saveas(figureHandle2,'images\approxImg2','jpg');
saveas(figureHandle3,'images\approxImg3','jpg');
saveas(figureHandle4,'images\approxImg4','jpg');
saveas(figureHandle5,'images\approxImg5','jpg');

fprintf('--> Approximated images saved and displayed \n\n')

%% PART D: Projecting and plotting in face space
% Enrolling (projecting) training images of Train A into Space A 
disp('-----------------------------------------------');
disp('  PART D: Projecting & plotting in face space');
disp('-----------------------------------------------');

% Project the image vectors into Space A
train_locations = projectImages(images, Means, Space);
save 'train_locations' train_locations;
 
% plotFirst3Coordinates(Locations, ThresholdIndividuals,ThresholdFaces)
% Plot the coordinates of the 5 training faces of the first 
% 5 training individuals in the space spanned by the first 3 eigenfaces
projImg1 = plotFirst3Coordinates(train_locations, 5, 5);
title('Projected training images in Space A');
saveas(projImg1,'images\projImg1','jpg');
fprintf('--> Projected image is saved \n\n')

disp('-----------------------------------------------');
disp('                END OF SCRIPT');
disp('-----------------------------------------------');










