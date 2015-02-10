fd=fopen('log_leds_quarto2.txt')
%Número de linhas
 nl=1;
%  strcell=textscan(fd,'%s',5)
%  %Temperature
%  temperature=strcell{1,1} (3,1)
%  %Time
%  time=strcell{1,1} (5,1)
 while(~feof(fd))
 strcell=textscan(fd,'%s %s %f %s %s')
 %Temperature
 %Time
 %time[nl]=strcell{1,1} (5,1);
 %strcell=textscan(fd,'%s',1)
 nl=nl+1
 end
fclose(fd);
figure
plot(1:size(strcell{1,3},1),strcell{1,3})
grid on
