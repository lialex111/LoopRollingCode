int test_func(int x) {
    return x;
}

int main() {
    int arr[] = {12, 34, 56, 78, 90, 43, 66, 100, 200};

    test_func(arr[1]);
    test_func(arr[2]);
    test_func(arr[4]);
    test_func(arr[8]);
}
