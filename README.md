
# vStl

This is a replacement for stl vector, with support or ranges.
I read a lot of stuff about iterator and ranges and here is my try to understan them better and try to make something useful out of it.

WORKING IN PROGRESS. DON'T CLONE

## Vector

 * MemoryBlock: manage memory for container.
 * GrowableArray: Array that can grow

Inspiration
https://github.com/electronicarts/EASTL
https://github.com/mendsley/tinystl
https://msharov.github.io/ustl/
https://github.com/think-cell/range
https://github.com/ericniebler/range-v3/
http://www.boost.org/doc/libs/1_65_0/libs/range/doc/html/index.html

### TODO:

 * FixedArray: Array allocated on heap, but all elemement are constructed at beginning.

#### Range based algorithms

 https://youtu.be/vrCtS6FDay8
 https://youtu.be/Yba5ncxzsnw

 * filter adaptor: remove the cenessity of all *_if algorithm
     - count_if(range, [](x){ return x < 4; }) == count(filter(range, [](x){ return x < 4; })
 * border
     - like iterator
     - cannot be dereferenced
     - if not has begin has element_before
     - if not at end has element after
     - range begin / end are border
     - all output iterator are borders
 * elements
     - can be nullable
     - for_each( range_of_elements(range), pred);
 * transform
 * for_each
 * map (like for_each, but create a new container)
 * zip
 * count / min / max / sum
 * all_of / any_of / none_of
 * take: keep the fist n element ... better name? trim?
 * take_last: keep the last n element
 * drop: remove the first n element
 * drop_back: remove the last n element
 * reverse
 * flattern
 * find return nullable element
 * find element that must exist
 * binary find
 * upper_bound / lower_bound return element before
 * Memory block as template? Trait? Allocator?
 * Compability with std::stl?

    vector<pair<int, char>> vec={{0, 'a'}, {0, 'b'}, {1, 'a'}, {1, 'b'}};
    auto it = find_if(v, [](A const& a) { return a.first == 0; });
    become..
    auto t = transform(vec, mem_fn(&pair<int, char>::first));
    auto it = find(t, 0);

    it have method to access base.

How should I concatenate transformation?
https://www.youtube.com/watch?v=alMAkmErrbo

    transform(filter(filter(t, pred1), pred2), [](x){ return x*x; });

    t | filter(pred1)
      | filter(pred2)
      | transform([](x){ return x*x; });
mmm sexy


