
set terminal pngcairo transparent enhanced font "arial,10" fontscale 1.0 size 600, 400
set output 'histograms.csv.png'  # Changed output filename to reflect CSV input


set key fixed right top vertical Right noreverse noenhanced autotitle nobox
set style data linespoints


set xtics border in scale 1,0.5 nomirror rotate by -45 autojustify
set xtics norangelimit 
set xtics ()


set title "US immigration from Europe by decade"
set xrange [ * : * ] noreverse writeback
set x2range [ * : * ] noreverse writeback
set yrange [ * : * ] noreverse writeback
set y2range [ * : * ] noreverse writeback
set zrange [ * : * ] noreverse writeback
set cbrange [ * : * ] noreverse writeback
set rrange [ * : * ] noreverse writeback

# Colorbox settings (if needed)
set colorbox vertical origin screen 0.9, 0.2 size screen 0.05, 0.6 front noinvert bdefault

# Plotting the data from a CSV file
set datafile separator ','  # Specify comma as the separator for CSV files
plot 'plant_data.csv' using 2:xtic(1) title columnheader(2), for [i=3:22] '' using i title columnheader(i)
