int main() {
    char arr[] = {'a','b','c','d','\0'};

    putchar(arr[0]);
    putchar(arr[1]);
    for (int i = 2; i < 4; ++i) {
        putchar(arr[i]);
    }
    // putchar(arr[2]);
    // putchar(arr[3]);
}
