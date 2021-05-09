clear % Limpa o workspace
clc % Limpada a janela de comandos 'Command window'
format ShortEng;%0.09 = 90e-003

sdisp = @(x) strrep(strtrim(evalc(sprintf('disp(%g)', x))), '.', ',');

fprintf('Passa banda RLC \n\n');

%https://www.dummies.com/education/science/science-electronics/create-band-pass-and-band-reject-filters-with-rlc-series-circuits/

f0 = 700e3;
L = 0.09e-3;
Bf = 16e3;

R = Bf * 2 * pi * L;
w0 = 2 * pi * f0;

C = (1/L)*(1/w0)^2;

wc1 = -(R/(2*L)) + sqrt((1 / (L*C)) + (R/(2*L))^2  );
wc2 = (R/(2*L)) + sqrt((1 / (L*C)) + (R/(2*L))^2  );

f1 = wc1 / (2*pi);
f2 = wc2 / (2*pi);

Q = w0 / ( Bf * 2 * pi);

fprintf('L =  %s \n', sdisp(L));
fprintf('C =  %s \n', sdisp(C));
fprintf('R =  %s \n', sdisp(R));
fprintf('f1 =  %s \n', sdisp(f1));
fprintf('f2 =  %s \n', sdisp(f2));
fprintf('Q =  %s \n', sdisp(Q));

fprintf('\n');

fprintf('Passa banda RC \n\n');

C = 447e-12;
Bf = 16e3;
f1 = f0 - (Bf/2);
f2 = f0 + (Bf/2);

R1 = 1 / (2 * pi * C * f1);
R2 = 1 / (2 * pi * C * f2);

fprintf('C1 = C2 =  %s \n', sdisp(C));
fprintf('R1 alta =  %s \n', sdisp(R1));
fprintf('R2 baixa =  %s \n', sdisp(R2));
fprintf('f1 =  %s \n', sdisp(f1));
fprintf('f2 =  %s \n', sdisp(f2));

fprintf('\n');
