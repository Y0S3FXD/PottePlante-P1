set title "Humidity (%)"
set xlabel "Measurement Number"
set ylabel "Value (%)"
set yrange [0:100]
plot 'data.tmp' using 1:3 with lines title "Humidity (%)"
