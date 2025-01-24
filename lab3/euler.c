#include <stdio.h>

int extended_euclid(int a, int b, int *x, int *y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    int x1, y1;
    int gcd = extended_euclid(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return gcd;
}

void multiplicative_inverse(int a, int m) {
    int x, y;
    int gcd = extended_euclid(a, m, &x, &y);
    if (gcd != 1) {
        printf("Multiplicative inverse of %d modulo %d does not exist.\n", a, m);
    } else {
        int positive_inverse = (x % m + m) % m;
        int negative_inverse = x;
        printf("The multiplicative inverse of %d modulo %d:\n", a, m);
        printf("Positive representation: %d\n", positive_inverse);
        printf("Negative representation: %d\n", negative_inverse);
    }
}

int main() {
    int a, m;
    printf("Enter the value of 'a' (integer to find inverse): ");
    scanf("%d", &a);
    printf("Enter the value of 'm' (modulo): ");
    scanf("%d", &m);

    if (m <= 0) {
        printf("Modulo value must be a positive integer.\n");
        return 1;
    }

    multiplicative_inverse(a, m);
    return 0;
}
