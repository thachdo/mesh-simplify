## Mesh Simplification with Quadirc Errors

I implemented a surface simplification algorithm which produces high quality approximations
of polygonal meshes. It uses iterative contractions of vertex pairs which minimize a quadric error metric, to
produce simplified meshes with high resemblance to the original. Based on the paper by M. Garland et al.

## Install
```
sudo apt install gcc-multilib
ln -s /usr/include/locale.h /usr/include/xlocale.h
sudo cp ./sys_files/* /usr/include/bits/
make
```

## Sample Results

We decimate edges to reduce the number of faces in these three examples. The number of faces is each shape is mentioned.

A cow `5500  ->  1500  ->  500  ->  200`

<img src="/images/cow_5500.png" width="47%"> <img src="/images/cow_1500.png" width="47%">
<img src="/images/cow_500.png" width="47%"> <img src="/images/cow_200.png" width="47%">

A bunny	`1000  ->  500  ->  200  ->  50`

<img src="/images/bunny_1k.png" width="47%"> <img src="/images/bunny_500.png" width="47%">
<img src="/images/bunny_200.png" width="47%"> <img src="/images/bunny_50.png" width="47%">

Beloved Homer Simpson `10200  ->  5000  ->  1000  ->  200`

<img src="/images/homer_10200.png" width="47%"> <img src="/images/homer_5000.png" width="47%">
<img src="/images/homer_1000.png" width="47%"> <img src="/images/homer_200.png" width="47%">
