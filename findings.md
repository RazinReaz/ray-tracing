- For some reason fast disregard in ray sphere intersection isnt working. there are noisy shadows.

# Fast disregard
The fast disregard is a technique to disregard the intersection of a ray and a sphere if the sphere is behind the ray.

After implementing the geometric solution, I found the output a lot noisy and there were shadows where there shouldnt be. Debugging the output (with colors) I found the shadows were caused because, the intersection point + the offset was
not enough to get out of the sphere. So the shadow was being cast by the sphere itself. I fixed this by changing the offset from `0.0001` to `0.1`. 

later on after a lot of maths, I figured out that the analytic solution can be optimized a lot. 
and the fast disregard can be incorported inside it. 

mainly, if c > 0 then the ray originated outside the sphere.
So if the projection of (center - r.origin) along the direction of the ray is negative, 
then the ray is pointing away from the sphere. 
so we don't need to calculate the expensive sqrt function and can return the code.

There can be two cases where we will not return from the code
- **c > 0 and  b > 0 (ray originated outside and ray is pointing towards the sphere)**: In this case, it is possible for the ray to not intersect the sphere. We can check this by checking if the discriminant is negative. There is a very subtle connection for the discriminant being negative with the geometric solution. I figured it out. It's some simple math really.
if the discriminant is negative, then the ray is pointing away from the sphere. So we can return from the code.
otherwise, we can calculate the two solutions and return the one with the smallest t.

- **c < 0 (ray originated inside the sphere)**: In this case, there will always be two solutions. 
The discriminant (b**2 - c (I know, but the math works if you do it manually)) will be positive and one of the two
solutions will be positive. We set t equal to the positive one (for now)

<!-- ## results
I ran the capturing code 4 times and took the average time.
### a lot of spheres in front of the viewport
| without fast disregard | with fast disregard |
|------------------------|---------------------|
| 4937.75 ms (avg)       | 5119.25 ms (avg)    |
### a lot of spheres behind the viewport
| without fast disregard | with fast disregard |
|------------------------|---------------------|
| 4069.25 ms (avg)       | 8893.75 ms (avg)    | -->


# TODO
- [x] config file
- [x] refactor with config
- [x] fix `fast disregard` in ray sphere intersection.
- [ ] back face culling 
- [ ] using const
- [x] ray inside sphere
- [ ] finish refactoring
    - testing needed
    - `include`/`headers` folder
    - fix the `metal` and `lambertian` classes staying in `interfaces.h`
- [x] anti aliasing
- [ ] optimize sqrt
- [ ] shared pointer for objects array
- [x] lambertian bounce implementation
    - recursion value needed
    - I don't know how to handle light source
        - Is the sky the only light? (According to ray tracing in one weekend)
        - I can watch the video from Josh's channel
        - I can watch the cherno playlist
- [x] materials
    - [x] attenuation
    - [x] material index in the object
- [x] gamma correction
- [ ] emmisive material
- [ ] bidirectional path tracing
- [ ] real time
    - will not go inside the ray
    - 