#  Matlab/Octave

Matlab (.m files) for Quantitative analysis (QA)  

## Convert .mat to .csv with cherry pick
Converting matlab to csv file is quite easy from **Ocatve** command line.  
Example carbig  
```
filename = "carbig.mat"
load(filename)
```
Checking the workspace panel I can see variables name
* Acceleration
* Cylinders
* Displacement
* Horsepower
* MPG
* ...

I decide to export (Acceleration,Horsepower,MPG)  

```
MYEXPORT = [Acceleration,Horsepower,MPG]
```

Time to check to ensure all datas to be correctly set (NaN hunt).  

```
tmpname = filename(1:length(filename)-3)
csvname = strcat(tmpname, 'csv')
save(csvname, "MYEXPORT")
```

Just done, no cry, edit csv file to check header struct (may be verbose), remove pointless lines if necessary.  
Fixing NaN can be done replacing NaN to 0.  
Fixing separator can be done replacing default space delimiter to the required one.  
