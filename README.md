# Ray tracing Redone
![Day scene](./code/assets/misc/shei.bmp)
![5 spheres](./code/assets/5%20spheres/scene%203%201000.bmp)
![reflection on sphere](./code/assets/misc/M=100.bmp)
![Night scene 1](./code/assets/night/500%20samples.bmp)
![Night Scene 2](./code/assets/night/1000%20samples%20emission%20.bmp)
![Night Scene 3](./code/assets/night/emit%20without%20atten.bmp


An offline ray tracer in OpenGL which supports
- Global illumination
- Materials (Metal, Lambertian)
- Emissive objects (light sources)
- Anti-aliasing
- Ambient occlusion


## Video demo
Samples per pixel (SPP) has been set as 10, 100, and 1000 for this video
<video src="./code/assets/video/5 spheres transition.mp4" width="600" height="600" controls></video>

For this video, spp has been set to 100, different materials were used for the spheres (lambertian, rough metal and smooth metal respectively)
<video src="./code/assets/video/one sphere.mp4" width="600" height="600" controls></video>