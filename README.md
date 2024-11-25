# multithreaded-image-matching-server

The purpose of this project is to construct a multithreaded client and a multithreaded server using POSIX threads (pthreads) in the C language. In this project, we used multithreading to improve the performance of a server that is programmed to accept an image from the user, match it against a database of known images, and return the closest matching image. In this project, we used the dispatcher-worker thread model.

#### Installation

Clone the repo

`git clone https://github.com/umutulay/multithreaded-image-matching-server.git`

## Usage

`$ make`

`$ ./server 8000 database 50 50 20`

`$ ./client img 8000 output/img`

## Authors

* Umut Tulay
* Sophia Mack
* Adit Kadepurkar
