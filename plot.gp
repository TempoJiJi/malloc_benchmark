reset
set ylabel 'time(sec)'
set xlabel 'Thread(n)'
set style fill solid
set key left
set title 'Allocator comparison'
set term png enhanced font 'Verdana,10'
set output 'runtime.png'
set grid
set yrange[0:0.05]

plot 'allocator_orig' using 2:xtic(1) w histogram title 'Original',\
     'bench_output' using 2:xtic(1) w histogram title 'Optimized'
