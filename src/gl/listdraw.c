#include "list.h"
#include "init.h"
#include "../glx/hardext.h"
#include "matrix.h"

void draw_renderlist(renderlist_t *list) {
    if (!list) return;
    // go to 1st...
    while (list->prev) list = list->prev;
    // ok, go on now, draw everything
//printf("draw_renderlist %p, size=%i, mode=%s(%s), ilen=%d, next=%p, color=%p, secondarycolor=%p fogcoord=%p\n", list, list->len, PrintEnum(list->mode), PrintEnum(list->mode_init), list->ilen, list->next, list->color, list->secondary, list->fogcoord);
    LOAD_GLES_FPE(glDrawArrays);
    LOAD_GLES_FPE(glDrawElements);
    LOAD_GLES_FPE(glVertexPointer);
    LOAD_GLES_FPE(glNormalPointer);
    LOAD_GLES_FPE(glColorPointer);
    LOAD_GLES_FPE(glTexCoordPointer);
    LOAD_GLES_FPE(glEnable);
    LOAD_GLES_FPE(glDisable);
    LOAD_GLES_FPE(glEnableClientState);
    LOAD_GLES_FPE(glDisableClientState);
    gl4es_glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

	int old_tex;
    GLushort *indices;
    static GLfloat *texgened[MAX_TEX] = {0};
    static int texgenedsz[MAX_TEX] = {0};
    int use_texgen[MAX_TEX] = {0};
    old_tex = glstate->texture.client;
    GLuint cur_tex = old_tex;
    GLint needclean[MAX_TEX] = {0};
    GLuint texture;
    bool stipple;
    
    do {
        // close if needed!
        if (list->open)
            list = end_renderlist(list);
        // push/pop attributes
        if (list->pushattribute)
            gl4es_glPushAttrib(list->pushattribute);
        if (list->popattribute)
            gl4es_glPopAttrib();
        call_list_t *cl = &list->calls;
        if (cl->len > 0) {
            for (int i = 0; i < cl->len; i++) {
                glPackedCall(cl->calls[i]);
            }
        }
        if(list->render_op) {
            switch(list->render_op) {
                case 1: gl4es_glInitNames(); break;
                case 2: gl4es_glPopName(); break;
                case 3: gl4es_glPushName(list->render_arg); break;
                case 4: gl4es_glLoadName(list->render_arg); break;
            }
        }
        if (list->fog_op) {
            gl4es_glFogfv(GL_FOG_COLOR, list->fog_val);
        }
        if (list->pointparam_op) {
            switch (list->pointparam_op) {
                case 1: // GL_POINT_DISTANCE_ATTENUATION 
                    gl4es_glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION , list->pointparam_val);
                    break;
            }
        }
        if (list->matrix_op) {
            switch (list->matrix_op) {
                case 1: // load
                    gl4es_glLoadMatrixf(list->matrix_val);
                    break;
                case 2: // mult
                    gl4es_glMultMatrixf(list->matrix_val);
                    break;
            }
        }
        if (list->set_tmu) {
            gl4es_glActiveTexture(GL_TEXTURE0+list->tmu);
        }
	    if (list->set_texture) {
            gl4es_glBindTexture(list->target_texture, list->texture);
        }
        // raster
        old_tex = glstate->texture.active;
        if (list->raster_op) {
            if (list->raster_op==1) {
                gl4es_glRasterPos3f(list->raster_xyz[0], list->raster_xyz[1], list->raster_xyz[2]);
            } else if (list->raster_op==2) {
                gl4es_glWindowPos3f(list->raster_xyz[0], list->raster_xyz[1], list->raster_xyz[2]);
            } else if (list->raster_op==3) {
                gl4es_glPixelZoom(list->raster_xyz[0], list->raster_xyz[1]);
            } else if ((list->raster_op&0x10000) == 0x10000) {
                gl4es_glPixelTransferf(list->raster_op&0xFFFF, list->raster_xyz[0]);
            }
        }
        if (list->raster) {
            rasterlist_t * r = list->raster;
            //glBitmap(r->width, r->height, r->xorig, r->yorig, r->xmove, r->ymove, r->raster);
            render_raster_list(list->raster);
        }
		// bitmaps
        if (list->bitmaps) {
            for (int i=0; i<list->bitmaps->count; i++) {
                bitmap_list_t *l = &list->bitmaps->list[i];
                gl4es_glBitmap(l->width, l->height, l->xorig, l->yorig, l->xmove, l->ymove, l->bitmap);
            }
        }

        if (list->material) {
            khash_t(material) *map = list->material;
            rendermaterial_t *m;
            kh_foreach_value(map, m,
                switch (m->pname) {
                    case GL_SHININESS:
                        gl4es_glMaterialf(m->face,  m->pname, m->color[0]);
                        break;
                    default:
                        gl4es_glMaterialfv(m->face, m->pname, m->color);
                }
            )
        }
        if (list->colormat_face)
            gl4es_glColorMaterial(list->colormat_face, list->colormat_mode);
        if (list->light) {
            khash_t(light) *lig = list->light;
            renderlight_t *m;
            kh_foreach_value(lig, m,
                switch (m->pname) {
                    default:
                        gl4es_glLightfv(m->which, m->pname, m->color);
                }
            )
        }
        if (list->lightmodel) {
            gl4es_glLightModelfv(list->lightmodelparam, list->lightmodel);
        }

        if (list->linestipple_op) {
            gl4es_glLineStipple(list->linestipple_factor, list->linestipple_pattern);
        }
		
        if (list->texenv) {
            khash_t(texenv) *tgn = list->texenv;
            rendertexenv_t *m;
            kh_foreach_value(tgn, m,
                gl4es_glTexEnvfv(m->target, m->pname, m->params);
            )
        }

        if (list->texgen) {
            khash_t(texgen) *tgn = list->texgen;
            rendertexgen_t *m;
            kh_foreach_value(tgn, m,
                gl4es_glTexGenfv(m->coord, m->pname, m->color);
            )
        }
        
        if (list->polygon_mode) {
            gl4es_glPolygonMode(GL_FRONT_AND_BACK, list->polygon_mode);}

        if (! list->len)
            continue;

        if (list->vert) {
            gles_glEnableClientState(GL_VERTEX_ARRAY);
            gles_glVertexPointer(4, GL_FLOAT, list->vert_stride, list->vert);
            glstate->clientstate[ATT_VERTEX] = 1;
        } else {
            gles_glDisableClientState(GL_VERTEX_ARRAY);
            glstate->clientstate[ATT_VERTEX] = false;
        }

        if (list->normal) {
            gles_glEnableClientState(GL_NORMAL_ARRAY);
            gles_glNormalPointer(GL_FLOAT, list->normal_stride, list->normal);
            glstate->clientstate[ATT_NORMAL] = 1;
        } else {
            gles_glDisableClientState(GL_NORMAL_ARRAY);
            glstate->clientstate[ATT_NORMAL] = 0;
        }
    
        indices = list->indices;

        if(glstate->raster.bm_drawing)
            bitmap_flush();
        if (list->color) {
            gles_glEnableClientState(GL_COLOR_ARRAY);
            glstate->clientstate[ATT_COLOR] = 1;
            if (glstate->enable.color_sum && (list->secondary) && hardext.esversion==1 && !list->use_glstate) {
                if(!list->final_colors) {
                    list->final_colors=(GLfloat*)malloc(list->len * 4 * sizeof(GLfloat));
                    if (indices) {
                        for (int i=0; i<list->ilen; i++)
                            for (int j=0; j<4; j++) {
                                const int k=indices[i]*4+j;
                                list->final_colors[k]=list->color[k] + list->secondary[k];
                            }
                    } else {
                        for (int i=0; i<list->len*4; i++)
                            list->final_colors[i]=list->color[i] + list->secondary[i];
                    }
                }
                gles_glColorPointer(4, GL_FLOAT, 0, list->final_colors);
            } else {
//printf("colors=%f, %f, %f, %f / %f, %f, %f, %f\n", list->color[0],list->color[1],list->color[2],list->color[3], list->color[4],list->color[5],list->color[6],list->color[7]);
                gles_glColorPointer(4, GL_FLOAT, list->color_stride, list->color);
            }
        } else {
            gles_glDisableClientState(GL_COLOR_ARRAY);
            glstate->clientstate[ATT_COLOR] = 0;
        }
        if(hardext.esversion > 1) {
            // secondary color only on ES2+
            if (glstate->enable.color_sum && (list->secondary)) {
                gles_glEnableClientState(GL_SECONDARY_COLOR_ARRAY);
                fpe_glSecondaryColorPointer(4, GL_FLOAT, list->secondary_stride, list->secondary);
                glstate->clientstate[ATT_SECONDARY] = 1;
            } else {
                fpe_glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
                glstate->clientstate[ATT_SECONDARY] = 0;
            }
            // fog coord only on ES2+
            if ((glstate->fog.coord_src==GL_FOG_COORD) && (list->fogcoord)) {
                gles_glEnableClientState(GL_FOG_COORD_ARRAY);
                fpe_glFogCoordPointer(GL_FLOAT, list->fogcoord_stride, list->fogcoord);
                glstate->clientstate[ATT_FOGCOORD] = 1;
            } else {
                fpe_glDisableClientState(GL_FOG_COORD_ARRAY);
                glstate->clientstate[ATT_FOGCOORD] = 0;
            }
        }
        #define TEXTURE(A) if (cur_tex!=A) {gl4es_glClientActiveTexture(A+GL_TEXTURE0); cur_tex=A;}
        stipple = false;
        if (! list->tex[0]) {
            // TODO: do we need to support GL_LINE_STRIP?
            if (list->mode == GL_LINES && glstate->enable.line_stipple) {
                stipple = true;
                gl4es_glPushAttrib(GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT);
                TEXTURE(0);
                gl4es_glEnable(GL_BLEND);
                gl4es_glEnable(GL_TEXTURE_2D);
                gl4es_glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                bind_stipple_tex();
                list->tex[0] = gen_stipple_tex_coords(list->vert, list->len);
            } 
        }
        #define RS(A, len) if(texgenedsz[A]<len) {free(texgened[A]); texgened[A]=malloc(4*sizeof(GLfloat)*len); texgenedsz[A]=len; } use_texgen[A]=1
        // cannot use list->maxtex because some TMU can be using TexGen or point sprites...
        if(hardext.esversion==1) {
            for (int a=0; a<hardext.maxtex; a++) {
                if(glstate->enable.texture[a]) {
                    const GLint itarget = get_target(glstate->enable.texture[a]);
                    needclean[a]=0;
                    use_texgen[a]=0;
                    if ((glstate->enable.texgen_s[a] || glstate->enable.texgen_t[a] || glstate->enable.texgen_r[a]  || glstate->enable.texgen_q[a])) {
                        TEXTURE(a);
                        RS(a, list->len);
                        gen_tex_coords(list->vert, list->normal, &texgened[a], list->len, &needclean[a], a, (list->ilen<list->len)?indices:NULL, (list->ilen<list->len)?list->ilen:0);
                    } else if ((list->tex[a]==NULL) && !(list->mode==GL_POINT && glstate->texture.pscoordreplace[a])) {
                        RS(a, list->len);
                        gen_tex_coords(list->vert, list->normal, &texgened[a], list->len, &needclean[a], a, (list->ilen<list->len)?indices:NULL, (list->ilen<list->len)?list->ilen:0);
                    }
                    // adjust the tex_coord now if needed, even on texgened ones
                    gltexture_t *bound = glstate->texture.bound[a][itarget];
                    if((list->tex[a] || (use_texgen[a] && !needclean[a])) && ((!(globals4es.texmat || glstate->texture_matrix[a]->identity)) || (bound->adjust))) {
                        if(!use_texgen[a]) {
                            RS(a, list->len);
                            if(list->tex_stride[a]) {
                                GLfloat *src = list->tex[a];
                                GLfloat *dst = texgened[a];
                                int stride = list->tex_stride[a]>>2;    // stride need to be a multiple of 4 (i.e. sizeof(GLfloat))
                                for (int ii=0; ii<list->len; ii++) {
                                    memcpy(dst, src, 4*sizeof(GLfloat));
                                    src+=stride;
                                    dst+=4;
                                }
                            } else
                                memcpy(texgened[a], list->tex[a], 4*sizeof(GLfloat)*list->len);
                        }
                        if (!(globals4es.texmat || glstate->texture_matrix[a]->identity))
                            tex_coord_matrix(texgened[a], list->len, getTexMat(a));
                        if (bound->adjust) {
                            tex_coord_npot(texgened[a], list->len, bound->width, bound->height, bound->nwidth, bound->nheight);
                        }
                    }
                }
                if ((list->tex[a] || (use_texgen[a] && !needclean[a]))/* && glstate->enable.texture[a]*/) {
                    TEXTURE(a);
                    if(!glstate->clientstate[ATT_MULTITEXCOORD0+a]) {
                        gles_glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                        glstate->clientstate[ATT_MULTITEXCOORD0+a] = 1;
                    }
                    gles_glTexCoordPointer(4, GL_FLOAT, (use_texgen[a])?0:list->tex_stride[a], (use_texgen[a])?texgened[a]:list->tex[a]);
                } else {
                    if (glstate->clientstate[ATT_MULTITEXCOORD0+a]) {
                        TEXTURE(a);
                        gles_glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                        glstate->clientstate[ATT_MULTITEXCOORD0+a] = 0;
                    } 
//else if (!glstate->enable.texgen_s[a] && glstate->enable.texture[a]) printf("LIBGL: texture[%i] without TexCoord, mode=0x%04X (init=0x%04X), listlen=%i\n", a, list->mode, list->mode_init, list->len);
                    
                }
                if (!IS_TEX2D(glstate->enable.texture[a]) && (IS_ANYTEX(glstate->enable.texture[a]))) {
                    TEXTURE(a);
                    gles_glEnable(GL_TEXTURE_2D);
                }
            }
        } else {
            // texture loop for ES2+ version
            for (int a=0; a<hardext.maxtex; a++) {
                if(list->tex[a]) {
                    TEXTURE(a);
                    if(!glstate->clientstate[ATT_MULTITEXCOORD0+a]) {
                        gles_glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                        glstate->clientstate[ATT_MULTITEXCOORD0+a] = 1;
                    }
                    gles_glTexCoordPointer(4, GL_FLOAT, list->tex_stride[a], list->tex[a]);
                } else {
                    if (glstate->clientstate[ATT_MULTITEXCOORD0+a]) {
                        TEXTURE(a);
                        gles_glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                        glstate->clientstate[ATT_MULTITEXCOORD0+a] = 0;
                    } 
                }
            }
        }
        if (glstate->texture.client != old_tex) TEXTURE(old_tex);
        #undef RS
        #undef TEXTURE

        realize_textures();
        
        GLenum mode;
        mode = list->mode;
        if ((glstate->polygon_mode == GL_LINE) && (mode>=GL_TRIANGLES))
			mode = GL_LINES;
		if ((glstate->polygon_mode == GL_POINT) && (mode>=GL_TRIANGLES))
			mode = GL_POINTS;

        if (indices) {
            if (glstate->render_mode == GL_SELECT) {
                pointer_state_t vtx;
                vtx.pointer = list->vert;
                vtx.type = GL_FLOAT;
                vtx.size = 4;
                vtx.stride = 0;
                select_glDrawElements(&vtx, list->mode, list->ilen, GL_UNSIGNED_SHORT, indices);
            } else {
                if (glstate->polygon_mode == GL_LINE && list->mode_init>=GL_TRIANGLES) {
                    int n, s;
                    int ilen = list->ilen;
                    GLushort ind_line[ilen*4+2];
                    int k=0;
                    switch (list->mode_init) {
                        case GL_TRIANGLES:
                            if(ilen>2) {
                                // 1 triangle -> 3 lines
                                for (int i = 0; i<ilen-2; i+=3) {
                                    ind_line[k++] = indices[i+0]; ind_line[k++] = indices[i+1];
                                    ind_line[k++] = indices[i+1]; ind_line[k++] = indices[i+2];
                                    ind_line[k++] = indices[i+2]; ind_line[k++] = indices[i+0];
                                }
                            }
                            break;
                        case GL_TRIANGLE_STRIP:
                            // first 3 points a triangle, then a 2 lines per new point
                            if (ilen>2) {
                                ind_line[k++] = indices[0]; ind_line[k++] = indices[1];
                                for (int i = 2; i<ilen; i++) {
                                    ind_line[k++] = indices[i-2]; ind_line[k++] = indices[i];
                                    ind_line[k++] = indices[i-1]; ind_line[k++] = indices[i];
                                }
                            }
                            break;
                        case GL_TRIANGLE_FAN:
                            // first 3 points a triangle, then a 2 lines per new point too
                            if (ilen>2) {
                                ind_line[k++] = indices[0]; ind_line[k++] = indices[1];
                                for (int i = 2; i<ilen; i++) {
                                    ind_line[k++] = indices[0]; ind_line[k++] = indices[i];
                                    ind_line[k++] = indices[i-1]; ind_line[k++] = indices[i];
                                }
                            }
                            break;
                        case GL_QUADS:
                            if (ilen>3) {
                                // 4 lines per quads, but dest may already be a triangles list...
                                if (list->mode == GL_TRIANGLE_FAN) {
                                    // just 1 Quad
                                    for (int i=0; i<4; i++) {
                                        ind_line[k++] = indices[i+0]; ind_line[k++] = indices[(i+1)%4];
                                    }
                                } else {
                                    // list of triangles, 2 per quads...
                                    for (int i=0; i<ilen-5; i+=6) {
                                        ind_line[k++] = indices[i+0]; ind_line[k++] = indices[i+1];
                                        ind_line[k++] = indices[i+1]; ind_line[k++] = indices[i+2];
                                        ind_line[k++] = indices[i+2]; ind_line[k++] = indices[i+5];
                                        ind_line[k++] = indices[i+5]; ind_line[k++] = indices[i+0];
                                    }
                                }
                            }
                            break;
                        case GL_QUAD_STRIP:
                            // first 4 points is a quad, then 2 points per new quad
                            if (ilen>3) {
                                ind_line[k++] = indices[0]; ind_line[k++] = indices[1];
                                for (int i = 2; i<ilen-1; i+=2) {
                                    ind_line[k++] = indices[i-1]; ind_line[k++] = indices[i];
                                    ind_line[k++] = indices[i-2]; ind_line[k++] = indices[i+1];
                                    ind_line[k++] = indices[i+0]; ind_line[k++] = indices[i+1];
                                }
                            }
                            break;
                        case GL_POLYGON:
                            // if polygons have been merged, then info is lost...
                            if (ilen) {
                                ind_line[k++] = indices[0]; ind_line[k++] = indices[1];
                                for (int i = 1; i<ilen; i++) {
                                    ind_line[k++] = indices[i-1]; ind_line[k++] = indices[i];
                                }
                                ind_line[k++] = indices[ilen-1]; ind_line[k++] = indices[0];
                            }
                            break;
                    }
                    gles_glDrawElements(mode, k, GL_UNSIGNED_SHORT, ind_line);
                } else {
                    gles_glDrawElements(mode, list->ilen, GL_UNSIGNED_SHORT, indices);
                }
            }
        } else {
            if (glstate->render_mode == GL_SELECT) {	
                pointer_state_t vtx;
                vtx.pointer = list->vert;
                vtx.type = GL_FLOAT;
                vtx.size = 4;
                vtx.stride = 0;
                select_glDrawArrays(&vtx, list->mode, 0, list->len);
            } else {
                int len = list->len;
                if ((glstate->polygon_mode == GL_LINE) && (list->mode_init>=GL_TRIANGLES)) {
                    int n, s;
                    
                    if(!list->ind_lines) {
                        GLushort *ind_line = list->ind_lines = (GLushort*)malloc(sizeof(GLushort)*len*4+2);
                        int k=0;
                        switch (list->mode_init) {
                            case GL_TRIANGLES:
                                // 1 triangle -> 3 lines
                                if(len>2) {
                                    for (int i = 0; i<len-2; i+=3) {
                                        ind_line[k++] = i+0; ind_line[k++] = i+1;
                                        ind_line[k++] = i+1; ind_line[k++] = i+2;
                                        ind_line[k++] = i+2; ind_line[k++] = i+0;
                                    }
                                }
                                break;
                            case GL_TRIANGLE_STRIP:
                                // first 3 points a triangle, then a 2 lines per new point
                                if (len>2) {
                                    ind_line[k++] = 0; ind_line[k++] = 1;
                                    for (int i = 2; i<len; i++) {
                                        ind_line[k++] = i-2; ind_line[k++] = i;
                                        ind_line[k++] = i-1; ind_line[k++] = i;
                                    }
                                }
                                break;
                            case GL_TRIANGLE_FAN:
                                // first 3 points a triangle, then a 2 lines per new point too
                                if (len>2) {
                                    ind_line[k++] = 0; ind_line[k++] = 1;
                                    for (int i = 2; i<len; i++) {
                                        ind_line[k++] = 0; ind_line[k++] = i;
                                        ind_line[k++] = i-1; ind_line[k++] = i;
                                    }
                                }
                                break;
                            case GL_QUADS:
                                if(len>3) {
                                    // 4 lines per quads, QUAD without indices means 1 single quad
                                    if (list->mode == GL_TRIANGLE_FAN) {
                                        // just 1 Quad
                                        for (int i=0; i<4; i++) {
                                            ind_line[k++] = i+0; ind_line[k++] = (i+1)%4;
                                        }
                                    } else {
                                        // list of triangles, 2 per quads...
                                        for (int i=0; i<len-5; i+=6) {
                                            ind_line[k++] = i+0; ind_line[k++] = i+1;
                                            ind_line[k++] = i+1; ind_line[k++] = i+2;
                                            ind_line[k++] = i+2; ind_line[k++] = i+5;
                                            ind_line[k++] = i+5; ind_line[k++] = i+0;
                                        }
                                    }
                                }
                                break;
                            case GL_QUAD_STRIP:
                                if(len>3) {
                                    // first 4 points is a quad, then 2 points per new quad
                                    if (len>2) {
                                        ind_line[k++] = 0; ind_line[k++] = 1;
                                    }
                                    for (int i = 2; i<len-1; i+=2) {
                                        ind_line[k++] = i-1; ind_line[k++] = i;
                                        ind_line[k++] = i-2; ind_line[k++] = i+1;
                                        ind_line[k++] = i+0; ind_line[k++] = i+1;
                                    }
                                }
                                break;
                            case GL_POLYGON:
                                // if polygons have been merged, then info is lost...
                                if (len) {
                                    ind_line[k++] = 0; ind_line[k++] = 1;
                                    for (int i = 1; i<len; i++) {
                                        ind_line[k++] = i-1; ind_line[k++] = i;
                                    }
                                    ind_line[k++] = len-1; ind_line[k++] = 0;
                                }
                                break;
                        }
                        list->ind_line = k;
                    }
					gles_glDrawElements(mode, list->ind_line, GL_UNSIGNED_SHORT, list->ind_lines);
                } else {
                    gles_glDrawArrays(mode, 0, len);
                }
            }
        }

        #define TEXTURE(A) if (cur_tex!=A) {gl4es_glClientActiveTexture(A+GL_TEXTURE0); cur_tex=A;}
        if(hardext.esversion==1)
            for (int a=0; a<hardext.maxtex; a++) {
                if (needclean[a]) {
                    TEXTURE(a);
                    gen_tex_clean(needclean[a], a);
                }
                if (!IS_TEX2D(glstate->enable.texture[a]) && (IS_ANYTEX(glstate->enable.texture[a]))) {
                    TEXTURE(a);
                    gles_glDisable(GL_TEXTURE_2D);
                }
            }
        if (glstate->texture.client!=old_tex)
            TEXTURE(old_tex);
        #undef TEXTURE

        if (stipple) {
            gl4es_glPopAttrib();
        }
        if(list->post_color) gl4es_glColor4fv(list->post_colors);
        if(list->post_normal) gl4es_glNormal3fv(list->post_normals);
    } while ((list = list->next));
    gl4es_glPopClientAttrib();
}
