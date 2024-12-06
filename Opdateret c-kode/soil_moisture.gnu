set title "Soil Moisture (%)"
set xlabel "Measurement Number"
set ylabel "Value (%)"
set yrange [0:100]
plot 'data.tmp' using 1:4 with lines title "Soil Moisture (%)"
