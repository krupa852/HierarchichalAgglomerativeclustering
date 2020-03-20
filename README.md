# HierarchichalAgglomerativeclustering
<br>1.Agglomerativeclustering.c - program to create 3 clusters using agglomerative clustering.</br>
<br>2.generate_test_data.sh - generates random 100 points with id label(number)</br>

<br>Steps to execute:></br>
<br>./generate_test_data.sh >filename.data</br>
<br>./a.out filename.data [number of datapoints]</br>
<br>$gnuplot</br>
<br>gnuplot>set terminal x11</br>
<br>gnuplot> set xrange [0:1500]</br>
<br>gnuplot> set yrange [0:1500]</br>
<br>gnuplot> set palette model RGB defined (0 "red", 1 "blue", 2 "green", 3 "black")</br>
<br>gnuplot> plot ("plot1.data")  with points pt 3 palette</br>
