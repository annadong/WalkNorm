function indexOfFreeze = NoEmptyArray(pitch)
    result = [];
    result = pitch  + abs(mean(pitch));
    x = (1:length(result));
    
    curresult = 0;
    prevresult = 0;
    nextresult = 0;
    
    indexOfFreeze = [];
    
    isMin = [0,0];
    isMax = [0,0];
    peakmeantime = 0;
    troughmeantime = 0;
    prevpeak = 0;
    prevtrough = 0;
    a = 0;
    b = 0;
    peakfreezing = 0;
    troughfreezing= 0;
    maximumValue = 0;
    minimumValue = 0;
    topThreshold = 0;
    lowThreshold = 0;
    curpeak = 0;
    curtrough = 0;
    indexOfFreeze1 =[];
    indexOfFreeze2 = [];
    hold on;
    for i = 1:length(result)-2
        prevresult = curresult;
        curresult = nextresult;
        nextresult = result(i);

        isMin(1) = [isMin(2)];  
        if((curresult - prevresult < 0) && (curresult - nextresult < 0 ) && (curresult < -5) )    
            isMin(2) = [1];
            curtrough = curresult;
        else
            isMin(2) = [0];
        end    
        

        if(isMin(2) == 1 && isMin(1) == 0)
            plot(x,result,i,curresult,'rx');
            troughmeantime = (i-prevtrough+troughmeantime*a)/(a+1);
            minimumValue = (curresult+minimumValue*a)/(a+1);
            if i-prevtrough < troughmeantime*0.5
                troughfreezing = troughfreezing + 1;
            end    
            prevtrough = i;
            a = a+1;
        end
        
        isMax(1) = [isMax(2)];    
        if((curresult - prevresult > 0) && (curresult - nextresult > 0 ) && (curresult > 5) )    
            isMax(2) = [1];
            curpeak = curresult;
        else
            isMax(2) = [0];
        end 
        
        if(isMax(2) == 1 && isMax(1) == 0)
            plot(x,result,i,curresult,'bx');
            peakmeantime = (i-prevpeak+peakmeantime*b)/(b+1);
            maximumValue = (curresult+maximumValue*b)/(b+1);
            topThreshold = (maximumValue-minimumValue)*0.75;
            lowThreshold = (maximumValue - minimumValue)*0.25;
                if i-prevpeak < peakmeantime*0.5
                   peakfreezing = peakfreezing + 1;
                end   
                if peakfreezing == troughfreezing && i > 50
                  indexOfFreeze1 = [indexOfFreeze1 i]
                end    
                if (lowThreshold < (curpeak - curtrough)) && ((curpeak - curtrough) < topThreshold)
                        indexOfFreeze2 = [indexOfFreeze2 i]
                end    
                
            prevpeak = i;
            b = b+1;
        end
            
    end
    
    
    hold off;
        
end