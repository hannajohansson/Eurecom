% Hanna Johansson ImSecu 2017-06-09
% Lab 2: Face eecognition using eigenfaces
% Exercise 3: Verification

% Obtained: our first verification results
% Question: "Is he/she the person he?she claims to be?"

close all;
%clear all;

% Add path to the public directory to access ready-made functions
addpath T:\courses\image\TpBiometry\public\Matlab

disp('-----------------------------------------------');
disp('  PART A: Computing client and impostor scores ');
disp('-----------------------------------------------');
% PART A: Computing client and impostor scores

% Project test images of Test A into Space A
test_locations = projectImages(test_images, Means, Space);
fprintf('--> Test images projected into Space A \n')

% Use the function 'verify' to compute the client and impostor scores
% [DistancesClients, DistancesImpostors] = verify(Models, Test, Threshold/FirstNEigenfaces)
[DistancesClients, DistancesImpostors] = verify(train_locations, test_locations, 5);
fprintf('--> Client and impostor scores computed \n');
save 'DistancesClients' DistancesClients;
save 'DistancesImpostors' DistancesImpostors;

% Plot the histograms of the client and impostor scores, on the same figure
histogram(DistancesClients, 'DisplayStyle', 'stairs');
hold on;
histogram(DistancesImpostors, 'DisplayStyle', 'stairs');
title('Client and impostor scores');
legend('Clients', 'Impostors');

fprintf('--> Histograms displayed \n\n');

% In order to better interpret the results --> rescale the histograms and
% plot their coordinates 
[YClients, XClients] = hist(-log(DistancesClients), 10);
[YImpostors, XImpostors] = hist(-log(DistancesImpostors), 10);
figure;
hold on;
scores_Fig = plot(XClients, YClients,'b', XImpostors,(YImpostors/19),'r');
title('Client and impostor scores');
legend('Clients', 'Impostors');

%% PART B: Plotting the Receiver Operating Characterisic (ROC) curve
disp('-----------------------------------------------');
disp('  PART B: Plotting the ROC curve');
disp('-----------------------------------------------');

% Compute  
%   the False Rejection Rate (FRR) and 
%   the False Acceptance Rate (FAR)
[FalseRejectionRates, FalseAcceptanceRates] = computeVerificationRates(DistancesClients, DistancesImpostors);
save 'FalseRejectionRates' FalseRejectionRates;
save 'FalseAcceptanceRates' FalseAcceptanceRates;

% Plot the results
figure;
ROC = plot(FalseRejectionRates, FalseAcceptanceRates);
title('ROC curve based on FRR and FAR');
xlabel('FalseRejectionRates (FRR)');
ylabel('FalseAcceptanceRates (FAR)');
fprintf('--> ROC curve displayed \n');

% Save the figure
saveas(ROC, 'images\ROC_curve','jpg');
fprintf('--> ROC curve saved \n\n');

%% PART C: Drawing the Equal Error Rate (ERR) curve
disp('-----------------------------------------------');
disp('  PART C: Drawing the ERR curve');
disp('-----------------------------------------------');


fprintf('--> Computing the ERR for different number of eigenfaces \n');
nmbr_Eigenfaces = zeros(length(50));
EqualErrorRate = zeros(length(50));
for i=1:100
    [DistancesClients, DistancesImpostors] = verify(train_locations, test_locations, i);
    nmbr_Eigenfaces(i) = i;
    % Compute the Equal Error Rate (EER)
    EqualErrorRate(i) = computeEER(DistancesClients, DistancesImpostors);
end
fprintf('--> Equal Error Rates computed \n');

figure;
ERR_fig = plot(nmbr_Eigenfaces, EqualErrorRate, '-');
title('The EER as a function of the number of eigenfaces');
fprintf('--> Equal Error Rates plotted \n');
xlabel('Eigenfaces');
ylabel('Equal Error Rate');

% Save the figure
saveas(ERR_fig, 'images\ERR_fig','jpg');
fprintf('--> Figure saved \n\n');

disp('-----------------------------------------------');
disp('                END OF SCRIPT');
disp('-----------------------------------------------');









