| Type         | Position          | Falloff   | Direction | Cutoff Angle |
|--------------|-------------------|-----------|-----------|--------------|
| Point Light  | (40, 100, -30)     | 2e-10     | -         | -            |
| Point Light  | (100, 20, -10)     | 2e-10     | -         | -            |
| Spotlight    | (300, 300, 150)    | 2e-09     | (0, 0, 0) | 1.0472       |
| Spotlight    | (300, 300, -150)   | 2e-09     | (0, 0, 0) | 1.309        |

| Type        | Size | Ambient | Diffuse | Reflection |
|-------------|------|---------|---------|------------|
| Checkerboard| 50   | 0.1     | 0.8     | 0.1        |

| Center            | Radius | Color        | Ambient | Diffuse | Specular | Reflection | Shininess |
|-------------------|--------|--------------|---------|---------|----------|------------|-----------|
| (0, 0, 0)         | 40     | 0.1, 0.8, 0.1| 0.1     | 0.2     | 0.2      | 0.5        | 30        |
| (80, 60, 0)       | 10     | 0.8, 0.3, 0.3| 0.05    | 0.1     | 0.25     | 0.6        | 30        |
| (80, 25, -90)     | 13     | 0, 0.7, 0.9  | 0.05    | 0.3     | 0.25     | 0.4        | 30        |
| (50, 10, 0)       | 10     | 0.8, 0.3, 0.9| 0.05    | 0.3     | 0.25     | 0.4        | 10        |
| (140, 69, -180)   | 40     | 0.2, 0.3, 0.9| 0.1     | 0.7     | 0.1      | 0.1        | 12        |
| (140, 69, 170)    | 15     | 0.2, 0.3, 0.9| 0.1     | 0.1     | 0.7      | 0.1        | 12        |
| (10, 20, -90)     | 20     | 0.9, 0.8, 0  | 0.1     | 0.5     | 0.3      | 0.1        | 12        |
| (90, 35, 60)      | 35     | 0.9, 0, 0.9  | 0.1     | 0.3     | 0.5      | 0.1        | 30        |
| (90, 35, -100)    | 35     | 0.9, 0, 0.9  | 0.2     | 0.4     | 0.1      | 0.3        | 30        |


- [x] generate ray function added [ 10.6 seconds ]
- [x] calculate color function added [ 18.48 seconds ]
- [x] mutex and parrallel processing added for each pixel [more than a minute]
- [x] mutex and parrallel processing added for horizontal blocks of pixels [6.308 seconds]