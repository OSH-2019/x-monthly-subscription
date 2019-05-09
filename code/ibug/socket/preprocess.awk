#!/usr/bin/awk -f

BEGIN {
    n = 0;
}
{
    if ($1) {
        a[n] = $1;
        n += 1;
        if (n >= 32) {
            printf "%.3f", a[n - 32];
            for (i = n - 31; i < n; i += 1) {
                printf " %.3f", a[i];
            }
            printf "\n";
        }
    }
}
