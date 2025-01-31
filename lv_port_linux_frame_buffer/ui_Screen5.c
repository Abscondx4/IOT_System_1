// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.2
// LVGL version: 8.3.11
// Project name: SquareLine_Project_2

#include "ui.h"

void ui_Screen5_screen_init(void)
{
    ui_Screen5 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen5, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Label11 = lv_label_create(ui_Screen5);
    lv_obj_set_width(ui_Label11, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label11, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label11, 0);
    lv_obj_set_y(ui_Label11, -151);
    lv_obj_set_align(ui_Label11, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label11, "忘记密码");
    lv_obj_set_style_text_align(ui_Label11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label11, &ui_font_HS4, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_TextArea5 = lv_textarea_create(ui_Screen5);
    lv_obj_set_width(ui_TextArea5, 390);
    lv_obj_set_height(ui_TextArea5, LV_SIZE_CONTENT);    /// 42
    lv_obj_set_x(ui_TextArea5, 0);
    lv_obj_set_y(ui_TextArea5, -93);
    lv_obj_set_align(ui_TextArea5, LV_ALIGN_CENTER);
    lv_textarea_set_placeholder_text(ui_TextArea5, "用户名");
    lv_textarea_set_one_line(ui_TextArea5, true);
    lv_obj_set_style_text_font(ui_TextArea5, &ui_font_HS4, LV_PART_MAIN | LV_STATE_DEFAULT);



    ui_Keyboard3 = lv_keyboard_create(ui_Screen5);
    lv_obj_set_width(ui_Keyboard3, 800);
    lv_obj_set_height(ui_Keyboard3, 170);
    lv_obj_set_x(ui_Keyboard3, 0);
    lv_obj_set_y(ui_Keyboard3, 152);
    lv_obj_set_align(ui_Keyboard3, LV_ALIGN_CENTER);

    ui_TextArea6 = lv_textarea_create(ui_Screen5);
    lv_obj_set_width(ui_TextArea6, 390);
    lv_obj_set_height(ui_TextArea6, LV_SIZE_CONTENT);    /// 42
    lv_obj_set_x(ui_TextArea6, 0);
    lv_obj_set_y(ui_TextArea6, -35);
    lv_obj_set_align(ui_TextArea6, LV_ALIGN_CENTER);
    lv_textarea_set_placeholder_text(ui_TextArea6, "新密码");
    lv_textarea_set_one_line(ui_TextArea6, true);
    lv_textarea_set_password_mode(ui_TextArea6, true);
    lv_obj_set_style_text_font(ui_TextArea6, &ui_font_HS4, LV_PART_MAIN | LV_STATE_DEFAULT);



    ui_Button7 = lv_btn_create(ui_Screen5);
    lv_obj_set_width(ui_Button7, 390);
    lv_obj_set_height(ui_Button7, 42);
    lv_obj_set_x(ui_Button7, 0);
    lv_obj_set_y(ui_Button7, 26);
    lv_obj_set_align(ui_Button7, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button7, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Button7, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Button7, lv_color_hex(0x5D5D5D), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label12 = lv_label_create(ui_Button7);
    lv_obj_set_width(ui_Label12, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label12, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label12, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label12, "重设");
    lv_obj_set_style_text_font(ui_Label12, &ui_font_HS4, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ImgButton3 = lv_imgbtn_create(ui_Screen5);
    lv_imgbtn_set_src(ui_ImgButton3, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_803566317, NULL);
    lv_imgbtn_set_src(ui_ImgButton3, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_803566317, NULL);
    lv_imgbtn_set_src(ui_ImgButton3, LV_IMGBTN_STATE_DISABLED, NULL, &ui__temporary_image, NULL);
    lv_imgbtn_set_src(ui_ImgButton3, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &ui__temporary_image, NULL);
    lv_imgbtn_set_src(ui_ImgButton3, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &ui__temporary_image, NULL);
    lv_imgbtn_set_src(ui_ImgButton3, LV_IMGBTN_STATE_CHECKED_DISABLED, NULL, &ui__temporary_image, NULL);
    lv_obj_set_height(ui_ImgButton3, 25);
    lv_obj_set_width(ui_ImgButton3, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x(ui_ImgButton3, -363);
    lv_obj_set_y(ui_ImgButton3, -210);
    lv_obj_set_align(ui_ImgButton3, LV_ALIGN_CENTER);

    lv_obj_add_event_cb(ui_TextArea5, ui_event_TextArea5, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_TextArea6, ui_event_TextArea6, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ImgButton3, ui_event_ImgButton3, LV_EVENT_ALL, NULL);

}
