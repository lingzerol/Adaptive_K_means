# Adaptive_K_means

advatage: Compared to normally K-means algorithm, the value of k we set is not very necessary. The new algorithm will automatically find
the value of which is close to the real one.


dataset: data02.txt

Adaptive K-means algorithm
perfomances:

k==10:
![Image_text](https://raw.githubusercontent.com/lingzerol/Adaptive_K_means/master/performance/Adaptive_K_means/data02/k%3D10.PNG)

k==8:
![Image_text](https://raw.githubusercontent.com/lingzerol/Adaptive_K_means/master/performance/Adaptive_K_means/data02/k%3D8.PNG)

k==6:
![Image_text](https://raw.githubusercontent.com/lingzerol/Adaptive_K_means/master/performance/Adaptive_K_means/data02/k%3D6.PNG)

k==4:
![Image_text](https://raw.githubusercontent.com/lingzerol/Adaptive_K_means/master/performance/Adaptive_K_means/data02/k%3D4.PNG)

Normal K-means algorithm
performance:

k==10:
![Image_text](https://raw.githubusercontent.com/lingzerol/Adaptive_K_means/master/performance/Normal_K_means/data02/k%3D10.PNG)

k==8:
![Image_text](https://raw.githubusercontent.com/lingzerol/Adaptive_K_means/master/performance/Normal_K_means/data02/k%3D8.PNG)

k==6:
![Image_text](https://raw.githubusercontent.com/lingzerol/Adaptive_K_means/master/performance/Normal_K_means/data02/k%3D6.PNG)

k==4:
![Image_text](https://raw.githubusercontent.com/lingzerol/Adaptive_K_means/master/performance/Normal_K_means/data02/k%3D4.PNG)

from the abrove performance, we can see that Adaptive K-mean is different from Normal K-means.
Adaptive K-means can separate the points to different cluster well no matter what value k takes.
And Normal K-means can not separate the points well because it is highly dependent on the k.
