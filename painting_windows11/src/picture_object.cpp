#include "myheader/picture_object.h"
#define DEBUG_MODE
namespace genetic_painting {
    PictureObject::PictureObject(PictureObject& a){
    }
    PictureObject::PictureObject(std::string& filepath) {
        this->tex_file_ = filepath;
    #ifdef DEBUG_MODE
        std::cout<<tex_file_<<"\n";
    #endif
        init_picture_object();
    }
    void PictureObject::init_picture_object() {
        texture_ = init_texture(tex_file_.c_str(), GL_RGBA, GL_RGB, GL_UNSIGNED_BYTE);
        set_vertices(0, 0, texture_height_, texture_width_);
    #ifdef DEBUG_MODE
        std::cout<<vertices_[0];
    #endif
        init_buffer_objects();
        shader_ = new Shader(c_vertex_vs_, c_frag_fs_);
        init_square_object2d();
        set_texture_unit();
        translate(0.0f, 0.0f, 0.0f);
        set_model_transform();
    }
}