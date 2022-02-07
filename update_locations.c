#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double *random_list;
int size;
int iters;
double *xs;
double *ys;
double *zs;
double *vx;
double *vy;
double *vz;
double t;
double chksum;

double *generate_random_list(int size, int bound) {
  random_list = (double *)malloc(sizeof(double)*size);
  for (int i=0; i<size; i++) {
    random_list[i] = rand();
  }
  return random_list;
}

void update_coords(double* xs, double* ys, double* zs, double* vx, double* vy, double* vz) {
    for (int i=0; i<sizeof(xs); i++) {
        xs[i] = xs[i] + vx[i];
        ys[i] = ys[i] + vy[i];
        zs[i] = zs[i] + vz[i];
  }
}

double time_update(int iters, double *xs, double *ys, double *zs, double *vx, double *vy, double *vz) {
  struct timespec start, stop;
  double time = 0;
  for (int i=0; i<iters; i++) {
    clock_gettime(CLOCK_MONOTONIC, &start);
    update_coords(xs, ys, zs, vx, vy, vz);
    clock_gettime(CLOCK_MONOTONIC, &stop);
    time += (stop.tv_sec - start.tv_sec)*1000000000 + (stop.tv_nsec - start.tv_nsec);  // in nanoseconds
  }
  return time;
}

double sum(double *s) {
  double sum = 0;
  for (int i=0; i<sizeof(s); i++) {
    sum = sum + s[i];
  }
  return sum;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Required arguments: vector_length(N) and iterations_num(M)");
    exit(-1);
  }
  size = (int)strtol(argv[1], NULL, 10);
  iters = (int)strtol(argv[2], NULL, 10);
  srand(size);
  xs = generate_random_list(size, 1000.);
  ys = generate_random_list(size, 1000.);
  zs = generate_random_list(size, 1000.);
  vx = generate_random_list(size, 1.);
  vy = generate_random_list(size, 1.);
  vz = generate_random_list(size, 1.);
  t = time_update(iters, xs, ys, zs, vx, vy, vz);
  chksum = sum(xs) + sum(ys) + sum(zs);
  printf("Mean time per coordinate: %fus\n", ((t / (size * iters)) / 1000));  // nanosecond to microsecond conversion added
  printf("Final checksum is: %f\n", chksum);
  exit(0);
}
