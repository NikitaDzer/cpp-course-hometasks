# Free List Allocator: First Fit vs Best Fit

## Benchmark Results

```
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
Allocator_FirstFit    3863458 ns      3844336 ns          182
Allocator_BestFit     3908603 ns      3905078 ns          182
```

First Fit ~3.86ms, Best Fit ~3.91ms per iteration.
