#include "myheader/brush_object.h"

namespace genetic_painting {
    BrushObject::BrushObject(BrushObject& a) {
        this->texture_idx_ = a.texture_idx_;
        init_brush_object();
    }
    BrushObject::BrushObject(int texture_idx) {
        texture_idx_ = texture_idx;
        init_brush_object();
    }
    void BrushObject::init_brush_object() {
        texture_ = init_texture(c_texture_filepath_[texture_idx_], GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
        set_vertices(0, 0, texture_height_, texture_height_);
        init_buffer_objects();
        shader_ = new Shader(c_vertex_shader_, c_fragment_shader_);
        init_square_object2d();
        set_texture_unit();
        translate(0.0f, 0.0f, -1.0f);
        set_model_transform();
    }
}
namespace genetic_painting {
    std::vector<BrushObject*> Brush::brushes_;
    void Brush::init_brushes() {
        Brush::brushes_.push_back(new BrushObject(0));
        Brush::brushes_.push_back(new BrushObject(1));
        Brush::brushes_.push_back(new BrushObject(2));
        Brush::brushes_.push_back(new BrushObject(3));
    }

    Brush::Brush() { }
    Brush::Brush(float x_translate, float y_translate, float z_translate, float scale, float angle, float brightness, float brush_idx) {
        x_translate_ = x_translate;
        y_translate_ = y_translate;
        z_translate_ = z_translate;
        scale_       = scale; 
        angle_       = angle; 
        brightness_  = brightness; 
        brush_idx_   = brush_idx;
    }
    void Brush::draw(glm::mat4& projection_translate) {
        brushes_[brush_idx_]->init_transform_matrixs();
        brushes_[brush_idx_]->set_projection_transform(projection_translate);
        brushes_[brush_idx_]->set_brightness(brightness_);
        brushes_[brush_idx_]->translate(x_translate_, y_translate_, z_translate_);
        brushes_[brush_idx_]->scale(scale_, scale_, 1.0f);
        brushes_[brush_idx_]->rotate(angle_);
        brushes_[brush_idx_]->set_model_transform();
        brushes_[brush_idx_]->draw();
    }
    void Brush::set_translate(float x_translate, float y_translate, float z_translate) {
        x_translate_ = x_translate;
        y_translate_ = y_translate;
        z_translate_ = z_translate;
    }
    void Brush::set_scale(float scale) {
        scale_       = scale; 
    }
    void Brush::set_rotate(float angle) {
        angle_       = angle; 
    }
    void Brush::set_brightness(float brightness) {
        brightness_  = brightness; 
    }
    void Brush::set_brush_randidx() {
        brush_idx_   = rand()%brushes_.size();
    }
}