

#include <include/matlab_bgl.h>
#include <stdio.h>

const char* errstr = "";

int test_1() {
  // line graph
  const mbglIndex n = 10;
  mbglIndex rp[] = {0,1,3,5,7,9,11,13,15,17,18};
  mbglIndex ci[] = {1,0,2,1,3,2,4,3,5,4,6,5,7,6,8,7,9,8};
  mbglIndex i[3*n-6];
  mbglIndex j[3*n-6];
  mbglIndex extra_edges;
  int rval;
  for (mbglIndex k=0; k<3*n-6; k++) { i[k]=j[k]=0; }
  rval= triangulate_graph(n, ci, rp, 1, 1, 1, i, j, &extra_edges);
  if (rval == 1) {
    errstr = "not planar";
    return -1;
  } else if (rval!=0) {
    errstr = "function error";
    return -1;
  }
  if (extra_edges + 9 != 3*n-6) {
    errstr = "insufficient edges";
    return -1;
  }
  for (mbglIndex k=0; k<extra_edges; k++) {
    if (i[k] == j[k]) {
      errstr = "invalid extra edges";
      return -1;
    }
  }
  return 0;
}

int test_2() {
  // line graph
  const mbglIndex n = 10;
  mbglIndex rp[] = {0,1,3,5,7,9,11,13,15,17,18};
  mbglIndex ci[] = {1,0,2,1,3,2,4,3,5,4,6,5,7,6,8,7,9,8};
  mbglIndex p[n];
  mbglDegreeType X[2*n];
  int rval;
  rval= chrobak_payne_straight_line_drawing(n, ci, rp, 0, 0, NULL, NULL, NULL, p, X);
  if (rval == 1) {
    errstr = "not planar";
    return -1;
  } else if (rval!=0) {
    errstr = "function error";
    return -1;
  }
  mbglIndex ei[3*n-6], ej[3*n-6], nedges;
  rval= chrobak_payne_straight_line_drawing(n, ci, rp, 0, 0, ei, ej, &nedges, p, X);
  if (rval == 1) {
    errstr = "not planar";
    return -1;
  } else if (rval!=0) {
    errstr = "function error";
    return -1;
  }
  if (nedges +9 != 3*n-6) {
    errstr = "incorrect extra edges";
    return -1;
  }
  return 0;
}

int test_3() {
  const mbglIndex n = 5;
  mbglIndex rp[] = {0,4,8,12,16,20};
  mbglIndex ci[] = {1,2,3,4,0,2,3,4,0,1,3,4,0,1,2,4,0,1,2,3};
  mbglIndex i[3*n-6];
  mbglIndex j[3*n-6];
  int is_planar= 0;
  mbglIndex nedges= 0;

  int rval;
  rval= boyer_myrvold_planarity_test(n, ci, rp, &is_planar,
      NULL, NULL, NULL, NULL, NULL);
  if (is_planar!=0) {
    errstr = "K5 is not planar";
    return -1;
  }
  if (rval != 0) {
    errstr = "function error";
    return -1;
  }
  rval= boyer_myrvold_planarity_test(n, ci, rp, &is_planar,
      i, j, &nedges, NULL, NULL);
  if (is_planar!=0) {
    errstr = "K5 is not planar";
    return -1;
  }
  if (rval != 0) {
    errstr = "function error";
    return -1;
  }
  return 0;
}

int test_4() {
  int rval;
  const mbglIndex n = 2;
  mbglIndex rp[] = {0,0,0};
  mbglIndex ci[] = {};
  mbglIndex i[6];
  mbglIndex j[6];

  int is_planar= 0;
  mbglIndex nedges= 0;

  rval= boyer_myrvold_planarity_test(n, ci, rp, &is_planar, i, j, &nedges, NULL, NULL);
  if (rval != 0) {
    errstr = "function error";
    return -1;
  }
  rval= triangulate_graph(n, ci, rp, 1, 0, 0, i, j, &nedges);
  if (rval != 0) {
    errstr = "function error";
    return -1;
  }
  rval= triangulate_graph(n, ci, rp, 1, 1, 0, i, j, &nedges);
  if (rval != 0) {
    errstr = "function error";
    return -1;
  }
  rval= triangulate_graph(n, ci, rp, 1, 1, 1, i, j, &nedges);
  if (nedges!=1) {
    errstr = "invalid extra edges";
    return -1;
  }
  return 0;
}

int main(int argc, char **argv) {
  int nfail = 0, ntotal = 0;
  int rval;

  const char* name;

  name= "make_maximal_planar";
  rval= test_1(); ntotal++;
  if (rval!= 0) { nfail++; printf("%20s  %50s\n", name, errstr); }
  else { printf("%20s  success\n", name); }

  name= "straight_line";
  rval= test_2(); ntotal++;
  if (rval!= 0) { nfail++; printf("%20s  %50s\n", name, errstr); }
  else { printf("%20s  success\n", name); }

  name= "planarity_test";
  rval= test_3(); ntotal++;
  if (rval!= 0) { nfail++; printf("%20s  %50s\n", name, errstr); }
  else { printf("%20s  success\n", name); }

  name= "empty_graphs";
  rval= test_4(); ntotal++;
  if (rval!= 0) { nfail++; printf("%20s  %50s\n", name, errstr); }
  else { printf("%20s  success\n", name); }

  printf("\n");
  printf("Total tests  : %3i\n", ntotal);
  printf("Total failed : %3i\n", nfail);

  return nfail!=0;
}
