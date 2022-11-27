int test_func(int x) {
    return x;
}

int main() {
    int arr[] = {12, 34, 56, 78, 90};
    test_func(arr[0]);
    test_func(arr[2]);
    test_func(arr[4]);
}

/*
instrNode

instr* inst                             -> ex: %call = call i32 @test_func(i32 noundef %1)
vector<something>                       -> {start, end}           -> ex: {arr[0], arr[4]} or {boxPtr->height, boxPtr->id}
int increment                           -> 2 or -1

*/