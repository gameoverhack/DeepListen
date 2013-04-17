// fragment
/*
uniform sampler2DRect yuyv_tex;
void main(){
    float g = gl_FragCoord.x;//texture2DRect(bars, texcoord.xy).r; // we need just one channel
    vec4 rgba = texture2DRect(yuyv_tex, gl_TexCoord[0].xy);
    vec3 finalcol; 
    if (g == 0.0) finalcol = convertYUVtoRGB(rgba.rga);
    else  finalcol = convertYUVtoRGB(rgba.bga);
    gl_FragColor = vec4(finalcol,1.0);
}
*/

//uniform sampler2DRect yuvTex;
//
//void main(){
//    vec4 yuvcolor; // f(xyz) -> yvu
//    float y, u , v;
//    vec4 rgbcolor;
//
//    y = texture2DRect(yuvTex, gl_TexCoord[0].xy).g;
//    u = texture2DRect(yuvTex, gl_TexCoord[0].xy).r-0.5;
//    v = texture2DRect(yuvTex, gl_TexCoord[0].xy).b-0.5;  
//
//    rgbcolor.r = 2.0 * (y/2.0 + 1.402/2.0 * u);
//    rgbcolor.g = 2.0 * (y/2.0 - 0.344136 * v/2.0 - 0.714136 * u/2.0);
//    rgbcolor.b = 2.0 * (y/2.0 + 1.773/2.0 * v);  
//
//    gl_FragColor = rgbcolor; 
//}

// modified from: http://www.fourcc.org/fccyvrgb.php and http://www.fourcc.org/source/YUV420P-OpenGL-GLSLang.c
uniform sampler2DRect yuvTex;
uniform int conversionType;

void main(void){
    
    float nx, ny, r, g, b, a, y, u, v;
    
    vec4 txl,ux,vx;
    
    nx = gl_TexCoord[0].x;
    ny = gl_TexCoord[0].y;
    
    y = texture2DRect(yuvTex, vec2(nx,ny)).g;
    u = texture2DRect(yuvTex, vec2(nx,ny)).b;
    v = texture2DRect(yuvTex, vec2(nx,ny)).r;
    
    // my version taken from wiki: http://en.wikipedia.org/wiki/YCbCr#ITU-R_BT.601_conversion
    y = 1.164383561643836 * (y - 0.0625);
    u = u - 0.5;
    v = v - 0.5;
    
    // default to ITU-R_BT.601_conversion
    float kb = 0.114;
    float kr = 0.299;
    float c = 2.276785714285714; //c = 255.0/112.0;
    
    if(conversionType == 709){
        // ITU-R_BT.709_conversion
        kb = 0.0722;
        kr = 0.2126;
    }

    r = y + c * (1.0 - kr) * v;
    g = y - c * (1.0 - kb) * (kb / (1.0 - kb - kr)) * u - c * (1.0 - kr) * (kr / (1.0 - kb - kr)) * v;
    b = y + c * (1.0 - kb) * u;
    
    gl_FragColor = vec4(r, g, b, 1.0);
    
    //    vec4 tl = texture2DRect(yuvTex, uv);
    //    vec4 tr = texture2DRect(textureSampler, uv + vec2(texelSize, 0));
    //    vec4 bl = texture2DRect(textureSampler, uv + vec2(0, texelSize));
    //    vec4 br = texture2DRect(textureSampler, uv + vec2(texelSize , texelSize));
    //    vec2 f = fract( uv.xy * textureSize ); // get the decimal part
    //    vec4 tA = mix( tl, tr, f.x ); // will interpolate the red dot in the image
    //    vec4 tB = mix( bl, br, f.x ); // will interpolate the blue dot in the image
    //    return mix( tA, tB, f.y );
    
    //y = 255.0/219.0 * (y - 16.0/256.0);
    //u = (u - 128.0/256.0);
    //v = (v - 128.0/256.0);
    
    // from: http://www.fourcc.org/fccyvrgb.php#mikes_answer
//    y = (y - 0.0625)/0.85546875;
//    u = (u - 0.5)/0.91796875;
//    v = (v - 0.5)/0.91796875;
    
//    y = 1.1643 * (y - 0.0625);
//    u = u - 0.5;
//    v = v - 0.5;

    // from http://www.martinreddy.net/gfx/faqs/colorconv.faq
    
    // European Y'U'V' (EBU) non-linear
//    r = y + 1.140 * v;
//    g = y - 0.396 * u - 0.581 * v;
//    b = y + 2.029 * u;

    // ITU.BT-601 Y'CbCr non-linear
//    r = y + 1.403 * v;
//    g = y - 0.344 * u - 0.714 * v;
//    b = y + 1.773 * u;
    
    // ITU.BT-709 HDTV non-linear
//    r = y + 1.5701 * v;
//    g = y - 0.1870 * u - 0.4664 * v;
//    b = y + 1.8556 * u;

    // JPEG Y'CbCr non-linear
//    r = y + 1.402 * v;
//    g = y - 0.34414 * u - 0.71414 * v;
//    b = y + 1.772 * u;
    
// from fourcc code example above
//    r = y + 1.5958 * v;
//    g = y - 0.39173 * u - 0.81290 * v;
//    b = y + 2.017 * u;
    
//    gl_FragColor = vec4(r, g, b, 1.0);
}

//const float textureSize = 720.0; //size of the texture
//const float texelSize = 1.0 / textureSize; //size of one texel 
//
//vec4 texture2DBilinear( sampler2DRect textureSampler, vec2 uv )
//{
//    // in vertex shaders you should use texture2DLod instead of texture2D
//    vec4 tl = texture2DRect(textureSampler, uv);
//    vec4 tr = texture2DRect(textureSampler, uv + vec2(texelSize, 0));
//    vec4 bl = texture2DRect(textureSampler, uv + vec2(0, texelSize));
//    vec4 br = texture2DRect(textureSampler, uv + vec2(texelSize , texelSize));
//    vec2 f = fract( uv.xy * textureSize ); // get the decimal part
//    vec4 tA = mix( tl, tr, f.x ); // will interpolate the red dot in the image
//    vec4 tB = mix( bl, br, f.x ); // will interpolate the blue dot in the image
//    return mix( tA, tB, f.y ); // will interpolate the green dot in the image
//}

/*
uniform sampler2DRect rgbsrc;

vec4 y_const = vec4( 0.257,  0.504,  0.098, 1);
vec4 v_const = vec4( 0.439, -0.368, -0.071, 1);
vec4 u_const = vec4(-0.148, -0.291, +0.439, 1);

void main(){
    // read two rgb pixels
    vec4 rgb1 = texture2DRect(rgbsrc, gl_TexCoord[0].xy);
    vec4 rgb2 = texture2DRect(rgbsrc, gl_TexCoord[1].xy);
    vec4 rgb12 = (rgb1 + rgb2) * 0.5;
    
    vec4 yuyv; // yuyv -> rgba
    
    yuyv.b = dot(rgb1,  y_const) +  16.0/256.0; // Y1
    yuyv.g = dot(rgb12, u_const) + 128.0/256.0; // v
    yuyv.r = dot(rgb2,  y_const) +  16.0/256.0; // Y2
    yuyv.a = dot(rgb12, v_const) + 128.0/256.0; // u
    
    gl_FragColor = yuyv;
}*/