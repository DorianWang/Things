
h = figure;
set(h,'name','Dorian Wang Matlab plot 1','numbertitle','off')




for i = -40:0.1:20
    if (i >= 9.0)
        plot(i, 1.5*sqrt(4*i)+10, '*'); hold on;
    elseif((i < 9.0) && (i >= 0.0))
        plot(i, 38/(11 - i), '*'); hold on;
    else
        plot(i, i*sin(i) + 38/11, '*'); hold on;
    end
end


xlabel('x')
ylabel('y')
title('Dorian Wang Matlab plot of piecewise function')
