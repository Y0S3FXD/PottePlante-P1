set title "Temperature (°C)"
set xlabel "Measurement Number"
set ylabel "Value (%)"
set yrange [0:100]
plot 'data.tmp' using 1:2 with lines title "Temperature (°C)"
