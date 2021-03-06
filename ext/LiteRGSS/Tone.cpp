#include "LiteRGSS.h"
#include "CTone_Element.h"

VALUE rb_cTone = Qnil;

#define TONE_PROTECT if(RDATA(self)->data == nullptr) \
{\
    rb_raise(rb_eRGSSError, "Freed Tone."); \
    return self; \
}

#define GET_TONE CTone_Element* tone; \
    Data_Get_Struct(self, CTone_Element, tone); \
    TONE_PROTECT \
    sf::Glsl::Vec4* tonev = tone->getTone();

void __Tone_Check_LinkedObject(CTone_Element* tone);

void rb_Tone_Free(void* data)
{
    CTone_Element* tone = reinterpret_cast<CTone_Element*>(data);
    if(tone)
    {
        CViewport_Element* viewport = tone->getElement();
        if(viewport != nullptr)
        {
            viewport->setLinkedRect(nullptr);
        }
        delete tone;
    }
}

VALUE rb_Tone_Alloc(VALUE klass)
{
    CTone_Element* tone = new CTone_Element();
    return Data_Wrap_Struct(klass, NULL, rb_Tone_Free, tone);
}

void Init_Tone()
{
    rb_cTone = rb_define_class_under(rb_mLiteRGSS, "Tone", rb_cObject);
    rb_define_alloc_func(rb_cTone, rb_Tone_Alloc);

    rb_define_method(rb_cTone, "initialize", _rbf rb_Tone_Initialize, -1);
    rb_define_method(rb_cTone, "set", _rbf rb_Tone_Initialize, -1);
    rb_define_method(rb_cTone, "initialize_copy", _rbf rb_Tone_InitializeCopy, 1);
    rb_define_method(rb_cTone, "red", _rbf rb_Tone_getRed, 0);
    rb_define_method(rb_cTone, "red=", _rbf rb_Tone_setRed, 1);
    rb_define_method(rb_cTone, "green", _rbf rb_Tone_getGreen, 0);
    rb_define_method(rb_cTone, "green=", _rbf rb_Tone_setGreen, 1);
    rb_define_method(rb_cTone, "blue", _rbf rb_Tone_getBlue, 0);
    rb_define_method(rb_cTone, "blue=", _rbf rb_Tone_setBlue, 1);
    rb_define_method(rb_cTone, "gray", _rbf rb_Tone_getGray, 0);
    rb_define_method(rb_cTone, "gray=", _rbf rb_Tone_setGray, 1);
    rb_define_method(rb_cTone, "==", _rbf rb_Tone_eql, 1);
    rb_define_method(rb_cTone, "===", _rbf rb_Tone_eql, 1);
    rb_define_method(rb_cTone, "eql?", _rbf rb_Tone_eql, 1);
    rb_define_method(rb_cTone, "to_s", _rbf rb_Tone_to_s, 0);
    rb_define_method(rb_cTone, "inspect", _rbf rb_Tone_to_s, 0);
    rb_define_method(rb_cTone, "_dump", _rbf rb_Tone_Save, 1);
    rb_define_singleton_method(rb_cTone, "_load", _rbf rb_Tone_Load, 1);
}

VALUE rb_Tone_Initialize(int argc, VALUE* argv, VALUE self)
{
    VALUE red, green, blue, alpha;
    rb_scan_args(argc, argv, "13", &red, &green, &blue, &alpha);
    GET_TONE
    if(RTEST(red))
        tonev->x = normalize_long(rb_num2long(red), -255, 255) / 255.0f;
    if(RTEST(green))
        tonev->y = normalize_long(rb_num2long(green), -255, 255) / 255.0f;
    if(RTEST(blue))
        tonev->z = normalize_long(rb_num2long(blue), -255, 255) / 255.0f;
    if(RTEST(alpha))
        tonev->w = normalize_long(rb_num2long(alpha), 0, 255) / 255.0f;
    __Tone_Check_LinkedObject(tone);
    return self;
}

VALUE rb_Tone_InitializeCopy(VALUE self, VALUE original)
{
    GET_TONE
    CTone_Element* toneo;
    Data_Get_Struct(original, CTone_Element, toneo);
    TONE_PROTECT
    if(RDATA(original)->data == nullptr)
        rb_raise(rb_eRGSSError, "Freed Tone.");
    tone_copy(tonev, toneo->getTone());
    return self;
}

VALUE rb_Tone_getRed(VALUE self)
{
    GET_TONE
    return rb_int2inum(static_cast<long>(tonev->x * 255.0f));
}

VALUE rb_Tone_setRed(VALUE self, VALUE val)
{
    GET_TONE
    tonev->x = normalize_long(rb_num2long(val), -255, 255) / 255.0f;
    __Tone_Check_LinkedObject(tone);
    return self;
}

VALUE rb_Tone_getGreen(VALUE self)
{
    GET_TONE
    return rb_int2inum(static_cast<long>(tonev->y * 255.0f));
}

VALUE rb_Tone_setGreen(VALUE self, VALUE val)
{
    GET_TONE
    tonev->y = normalize_long(rb_num2long(val), -255, 255) / 255.0f;
    __Tone_Check_LinkedObject(tone);
    return self;
}

VALUE rb_Tone_getBlue(VALUE self)
{
    GET_TONE
    return rb_int2inum(static_cast<long>(tonev->z * 255.0f));
}

VALUE rb_Tone_setBlue(VALUE self, VALUE val)
{
    GET_TONE
    tonev->z = normalize_long(rb_num2long(val), -255, 255) / 255.0f;
    __Tone_Check_LinkedObject(tone);
    return self;
}

VALUE rb_Tone_getGray(VALUE self)
{
    GET_TONE
    return rb_int2inum(static_cast<long>(tonev->w * 255.0f));
}

VALUE rb_Tone_setGray(VALUE self, VALUE val)
{
    GET_TONE
    tonev->w = normalize_long(rb_num2long(val), 0, 255) / 255.0f;
    __Tone_Check_LinkedObject(tone);
    return self;
}

VALUE rb_Tone_eql(VALUE self, VALUE other)
{
    if(rb_obj_is_kind_of(other, rb_cTone) != Qtrue)
        return Qfalse;
    GET_TONE
    CTone_Element* otone;
    Data_Get_Struct(other, CTone_Element, otone);
    if(otone == nullptr)
        return Qfalse;
    sf::Glsl::Vec4* otonev = otone->getTone();
    if(tonev->x != otonev->x)
        return Qfalse;
    if(tonev->y != otonev->y)
        return Qfalse;
    if(tonev->z != otonev->z)
        return Qfalse;
    if(tonev->w != otonev->w)
        return Qfalse;
    return Qtrue;
}

VALUE rb_Tone_to_s(VALUE self)
{
    GET_TONE
    return rb_sprintf("(%d, %d, %d, %d)", static_cast<int>(tonev->x * 255), 
        static_cast<int>(tonev->y * 255),
        static_cast<int>(tonev->z * 255),
        static_cast<int>(tonev->w * 255));
}

VALUE rb_Tone_Load(VALUE self, VALUE str)
{
    rb_check_type(str, T_STRING);
    VALUE arr[4];
    if(RSTRING_LEN(str) < (sizeof(double) * 4))
    {
        arr[3] = arr[2] = arr[1] = arr[0] = LONG2FIX(0);
        return rb_class_new_instance(4, arr, self);
    }
    double* tone_data = reinterpret_cast<double*>(RSTRING_PTR(str));
    arr[0] = rb_int2inum(static_cast<long>(tone_data[0]));
    arr[1] = rb_int2inum(static_cast<long>(tone_data[1]));
    arr[2] = rb_int2inum(static_cast<long>(tone_data[2]));
    arr[3] = rb_int2inum(static_cast<long>(tone_data[3]));
    return rb_class_new_instance(4, arr, self);
}

VALUE rb_Tone_Save(VALUE self, VALUE limit)
{
    GET_TONE
    double tone_data[4];
    tone_data[0] = static_cast<double>(tonev->x * 255);
    tone_data[1] = static_cast<double>(tonev->y * 255);
    tone_data[2] = static_cast<double>(tonev->z * 255);
    tone_data[3] = static_cast<double>(tonev->w * 255);
    return rb_str_new(reinterpret_cast<const char*>(tone_data), sizeof(double) * 4);
}

void __Tone_Check_LinkedObject(CTone_Element* tone)
{
    CViewport_Element* view = tone->getElement();
    if(view == nullptr)
        return;
    tone_copy(view->getTone(), tone->getTone());
	view->updatetone();
}