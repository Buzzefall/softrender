# Software Renderer
Software-rendered demoscene. Based on rasterisation algorithm. 

**C++17** required.

## Usage
Execute binary following with arbitrary N > 1 paths to objects separated by space (only right slashes / for object names):

`<bin_filename> <obj_folder_path>/<obj_name>/<obj_name>.obj [[<obj2_folder_path>/<obj2_name>/<obj2_name>.obj] ...]`

**Example**:

`softrender.exe obj/african_head/african_head.obj obj/african_head/african_head_eye_inner.obj obj/african_head/african_head_eye_outer.obj`
