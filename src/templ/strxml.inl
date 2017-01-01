/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-14  */
/*     #######          ###    ###      [CORE]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: XXXX-XX-XX  */
/*    ####  ##          ###    ###                  ###  ~~~~~~~~~~~~~~~~~~  */
/*   ###       ### ###  ###    ###    ####    ####  ###   ##  +-----------+  */
/*  ####       ######## ##########  #######  ###### ###  ###  |  A NEW C  |  */
/*  ###        ######## ########## ########  ###### ### ###   | FRAMEWORK |  */
/*  ###     ## #### ### ########## ###  ### ###     ######    |  FOR ALL  |  */
/*  ####   ### ###  ### ###    ### ###  ### ###     ######    | PLATFORMS |  */
/*  ########## ###      ###    ### ######## ####### #######   |  AND ALL  |  */
/*   #######   ###      ###    ### ########  ###### ###  ###  | COMPILERS |  */
/*    #####    ###      ###    ###  #### ##   ####  ###   ##  +-----------+  */
/*  =======================================================================  */
/*  >>>>>>>>>>>>>>>>>>>>> CrHack XML 解析器函数库模板 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

/*
=======================================
    生成 XML 解析器T
=======================================
*/
CR_API sXMLx*
CR_FUW(xml_parse) (
  __CR_IN__ const XCHAR*    str
    )
{
    sXMLx*      xml;
    XCHAR*      nxt;
    XCHAR*      tmp;
    leng_t      cnt;
    sXNODEx*    node;

    /* 计算标签个数 */
    for (cnt = 0, tmp = (XCHAR*)str; *tmp != CR_XN(NIL); tmp++)
    {
        /* 搜索标签 */
        if (*tmp != CR_XC('<'))
            continue;
        nxt = tmp;

        /* 跳过注释 */
        tmp = CR_FAW(skip_xml_cmt)(tmp);
        if (tmp != nxt) {
            if (tmp-- == NULL)
                return (NULL);
            continue;
        }

        /* 查找标签结尾 */
        nxt = CR_FUW(str_esc_chr2)(tmp + 1, CR_XC('<'), CR_XC('>'));
        if (nxt == NULL)
            return (NULL);

        /* 遇到另一个标签开始 */
        if (*nxt == CR_XC('<'))
            return (NULL);
        cnt++;
        tmp = nxt;
    }

    /* 至少要有两个标签 */
    if (cnt <= 1)
        return (NULL);
    xml = struct_new(sXMLx);
    if (xml == NULL)
        return (NULL);

    /* 分配内存 */
    xml->text = tmp = CR_FAW(str_dup)(str);
    if (xml->text == NULL) {
        mem_free(xml);
        return (NULL);
    }
    xml->nodes = mem_talloc(cnt, sXNODEx);
    if (xml->nodes == NULL) {
        mem_free(xml->text);
        mem_free(xml);
        return (NULL);
    }
    mem_tzero(xml->nodes, cnt, sXNODEx);

    /* 解析标签 */
    for (xml->count = 0; *tmp != CR_XN(NIL); tmp++)
    {
        /* 搜索标签 */
        if (*tmp != CR_XC('<'))
            continue;

        /* 跳过注释 */
        nxt = CR_FAW(skip_xml_cmt)(tmp);
        if (nxt != tmp) {
            tmp = nxt - 1;
            continue;
        }
        node = &xml->nodes[xml->count];

        *tmp++ = CR_XN(NIL);
        node->name = tmp;

        /* 查找属性 */
        for (;; tmp++)
        {
            /* 跳过转义 */
            if (*tmp == CR_XC('\\'))
                 tmp++;
            else
            if (*tmp == CR_XC('>') || CR_FAW(is_space)(*tmp))
                break;
        }

        if (*tmp != CR_XC('>'))
        {
            /* 有属性, 再看是否封闭 */
            *tmp++ = CR_XN(NIL);
            tmp = CR_FAW(skip_space)(tmp);
            if (tmp[0] == CR_XC('/') && tmp[1] == CR_XC('>'))
            {
                tmp += 1;
                node->attr = NULL;
            }
            else
            {
                /* 查找标记结束 */
                node->attr = tmp;
                tmp = CR_FUW(str_esc_chr)(tmp, CR_XC('>'));
            }
        }
        else
        {
            /* 无属性 */
            node->attr = NULL;
        }
        *tmp = CR_XN(NIL);

        /* 测试封闭属性 */
        if (*(tmp - 1) == CR_XC('/'))
        {
            if (node->name[0] == CR_XC('/'))
            {
                /* 结尾标签无法封闭 */
                CR_FUW(xml_close)(xml);
                return (NULL);
            }
            node->node = NULL;
            node->closed = TRUE;
        }
        else
        {
            node->node = tmp + 1;
            node->closed = FALSE;
        }

        /* 建立影子属性串 */
        if (node->attr != NULL)
        {
            *(node->attr - 1) = CR_XC(' ');
            node->shadow = CR_FAW(str_dup)(node->attr - 1);
            if (node->shadow == NULL) {
                CR_FUW(xml_close)(xml);
                return (NULL);
            }
            *(node->attr - 1) = CR_XN(NIL);

            /* 非法属性串 */
            if (CR_FUW(str_esc_empty)(node->shadow) == NULL) {
                CR_FUW(xml_close)(xml);
                return (NULL);
            }
        }
        else
        {
            node->shadow = NULL;
        }
        xml->count++;
    }
    return (xml);
}

/*
=======================================
    释放 XML 解析器T
=======================================
*/
CR_API void_t
CR_FUW(xml_close) (
  __CR_IN__ sXMLx*  xml
    )
{
    leng_t  idx;

    for (idx = 0; idx < xml->count; idx++)
        TRY_FREE(xml->nodes[idx].shadow);
    mem_free(xml->nodes);
    mem_free(xml->text);
    mem_free(xml);
}

/*
=======================================
    从文本生成 XML 解析器T
=======================================
*/
CR_API sXMLx*
CR_FUW(xml_from_text) (
  __CR_IN__ const ansi_t*   text
    )
{
    sXMLx*  xml;
    XCHAR*  str;

#if defined(_CR_BUILD_ANSI_)
    str = text_to_utf8(text, NULL, TRUE);
#else   /* (_CR_BUILD_WIDE_) */
    str = text_to_utf16(text, NULL, TRUE);
#endif
    if (str == NULL)
        return (NULL);
    xml = CR_FUW(xml_parse)(str);
    mem_free(str);
    return (xml);
}

/*
=======================================
    从 XML 节点读取整数T
=======================================
*/
CR_API uint_t
CR_FUW(xml_node_intx) (
  __CR_IN__ uint_t      def,
  __CR_IO__ sXNODEx*    node
    )
{
    XCHAR*  tmp;

    tmp = CR_FUW(xml_node_buffer)(node);
    if (tmp == NULL)
        return (def);
    return (CR_FAW(str2intx)(tmp, NULL));
}

/*
=======================================
    从 XML 节点读取32位整数T
=======================================
*/
CR_API int32u
CR_FUW(xml_node_intx32) (
  __CR_IN__ int32u      def,
  __CR_IO__ sXNODEx*    node
    )
{
    XCHAR*  tmp;

    tmp = CR_FUW(xml_node_buffer)(node);
    if (tmp == NULL)
        return (def);
    return (CR_FAW(str2intx32)(tmp, NULL));
}

/*
=======================================
    从 XML 节点读取64位整数T
=======================================
*/
CR_API int64u
CR_FUW(xml_node_intx64) (
  __CR_IN__ int64u      def,
  __CR_IO__ sXNODEx*    node
    )
{
    XCHAR*  tmp;

    tmp = CR_FUW(xml_node_buffer)(node);
    if (tmp == NULL)
        return (def);
    return (CR_FAW(str2intx64)(tmp, NULL));
}

/*
=======================================
    从 XML 节点读取实数T
=======================================
*/
CR_API real_t
CR_FUW(xml_node_real) (
  __CR_IN__ real_t      def,
  __CR_IO__ sXNODEx*    node
    )
{
    XCHAR*  tmp;

    tmp = CR_FUW(xml_node_buffer)(node);
    if (tmp == NULL)
        return (def);
    return (CR_FAW(str2real)(tmp, NULL));
}

/*
=======================================
    从 XML 节点读取32位实数T
=======================================
*/
CR_API fp32_t
CR_FUW(xml_node_fp32) (
  __CR_IN__ fp32_t      def,
  __CR_IO__ sXNODEx*    node
    )
{
    XCHAR*  tmp;

    tmp = CR_FUW(xml_node_buffer)(node);
    if (tmp == NULL)
        return (def);
    return (CR_FAW(str2fp32)(tmp, NULL));
}

/*
=======================================
    从 XML 节点读取64位实数T
=======================================
*/
CR_API fp64_t
CR_FUW(xml_node_fp64) (
  __CR_IN__ fp64_t      def,
  __CR_IO__ sXNODEx*    node
    )
{
    XCHAR*  tmp;

    tmp = CR_FUW(xml_node_buffer)(node);
    if (tmp == NULL)
        return (def);
    return (CR_FAW(str2fp64)(tmp, NULL));
}

/*
=======================================
    从 XML 节点读取转义字符串T
=======================================
*/
CR_API XCHAR*
CR_FUW(xml_node_string) (
  __CR_IO__ sXNODEx*    node
    )
{
    XCHAR*  tmp;

    tmp = CR_FUW(xml_node_buffer)(node);
    if (tmp == NULL)
        return (NULL);
    tmp = CR_FAW(skip_space)(tmp);
    return (CR_FUW(str_esc_dup)(tmp, &node->size, NULL));
}

/*
=======================================
    从 XML 节点读取值指针T
=======================================
*/
CR_API XCHAR*
CR_FUW(xml_node_buffer) (
  __CR_IO__ sXNODEx*    node
    )
{
    if (node->closed ||
        node->name[0] == CR_XC('/')) {
        node->found = FALSE;
        return (NULL);
    }
    node->found = TRUE;
    return (node->node);
}

/*
=======================================
    从 XML 节点属性读取整数T
=======================================
*/
CR_API uint_t
CR_FUW(xml_attr_intx) (
  __CR_IN__ const XCHAR*    name,
  __CR_IN__ uint_t          def,
  __CR_IO__ sXNODEx*        node
    )
{
    XCHAR*  tmp;

    tmp = CR_FUW(xml_attr_buffer)(name, node);
    if (tmp == NULL)
        return (def);
    return (CR_FAW(str2intx)(tmp, NULL));
}

/*
=======================================
    从 XML 节点属性读取32位整数T
=======================================
*/
CR_API int32u
CR_FUW(xml_attr_intx32) (
  __CR_IN__ const XCHAR*    name,
  __CR_IN__ int32u          def,
  __CR_IO__ sXNODEx*        node
    )
{
    XCHAR*  tmp;

    tmp = CR_FUW(xml_attr_buffer)(name, node);
    if (tmp == NULL)
        return (def);
    return (CR_FAW(str2intx32)(tmp, NULL));
}

/*
=======================================
    从 XML 节点属性读取64位整数T
=======================================
*/
CR_API int64u
CR_FUW(xml_attr_intx64) (
  __CR_IN__ const XCHAR*    name,
  __CR_IN__ int64u          def,
  __CR_IO__ sXNODEx*        node
    )
{
    XCHAR*  tmp;

    tmp = CR_FUW(xml_attr_buffer)(name, node);
    if (tmp == NULL)
        return (def);
    return (CR_FAW(str2intx64)(tmp, NULL));
}

/*
=======================================
    从 XML 节点属性读取实数T
=======================================
*/
CR_API real_t
CR_FUW(xml_attr_real) (
  __CR_IN__ const XCHAR*    name,
  __CR_IN__ real_t          def,
  __CR_IO__ sXNODEx*        node
    )
{
    XCHAR*  tmp;

    tmp = CR_FUW(xml_attr_buffer)(name, node);
    if (tmp == NULL)
        return (def);
    return (CR_FAW(str2real)(tmp, NULL));
}

/*
=======================================
    从 XML 节点属性读取32位实数T
=======================================
*/
CR_API fp32_t
CR_FUW(xml_attr_fp32) (
  __CR_IN__ const XCHAR*    name,
  __CR_IN__ fp32_t          def,
  __CR_IO__ sXNODEx*        node
    )
{
    XCHAR*  tmp;

    tmp = CR_FUW(xml_attr_buffer)(name, node);
    if (tmp == NULL)
        return (def);
    return (CR_FAW(str2fp32)(tmp, NULL));
}

/*
=======================================
    从 XML 节点属性读取64位实数T
=======================================
*/
CR_API fp64_t
CR_FUW(xml_attr_fp64) (
  __CR_IN__ const XCHAR*    name,
  __CR_IN__ fp64_t          def,
  __CR_IO__ sXNODEx*        node
    )
{
    XCHAR*  tmp;

    tmp = CR_FUW(xml_attr_buffer)(name, node);
    if (tmp == NULL)
        return (def);
    return (CR_FAW(str2fp64)(tmp, NULL));
}

/*
=======================================
    从 XML 节点属性读取转义字符串T
=======================================
*/
CR_API XCHAR*
CR_FUW(xml_attr_string) (
  __CR_IN__ const XCHAR*    name,
  __CR_IO__ sXNODEx*        node
    )
{
    XCHAR*  tmp;

    tmp = CR_FUW(xml_attr_buffer)(name, node);
    if (tmp == NULL)
        return (NULL);
    return (CR_FUW(str_esc_dup)(tmp - 1, &node->size, NULL));
}

/*
=======================================
    从 XML 节点属性读取值指针T
=======================================
*/
CR_API XCHAR*
CR_FUW(xml_attr_buffer) (
  __CR_IN__ const XCHAR*    name,
  __CR_IO__ sXNODEx*        node
    )
{
    XCHAR*  tmp;
    leng_t  len;

    if (node->shadow != NULL)
    {
        tmp = node->shadow;
        len = CR_FAW(str_len)(name);

        /* 查找属性名 */
        for (;;)
        {
            /* 大小写无关 */
            tmp = CR_FAW(str_strI)(tmp, name);
            if (tmp == NULL)
                goto _failed;

            /* 前面必须被空白打断 */
            if (CR_FAW(is_space)(*(tmp - 1))) {
                tmp = CR_FAW(skip_space)(tmp + len);
                if (*tmp == CR_XC('='))
                    break;
            }
            else {
                tmp += len;
            }
        }

        tmp = CR_FAW(skip_space)(tmp + 1);
        tmp = node->attr + (tmp - node->shadow) - 1;

        if (*tmp == CR_XC('\"')) {
            node->found = TRUE;
            return (tmp + 1);
        }
    }

_failed:
    node->found = FALSE;
    return (NULL);
}

/*
=======================================
    检查字符串是否合法T
=======================================
*/
CR_API bool_t
CR_FUW(xml_check_string) (
  __CR_IN__ const XCHAR*    str
    )
{
    leng_t  idx;
    leng_t  len = CR_FAW(str_len)(str);

    for (idx = 0; idx < len; idx++)
    {
        if (str[idx] == CR_XC('\\'))
            idx++;
        else
        if (str[idx] == CR_XC('<') ||
            str[idx] == CR_XC('>') || str[idx] == CR_XC('\"'))
            return (FALSE);
    }
    return (TRUE);
}

/*
=======================================
    跳过当前 XML 标签T
=======================================
*/
CR_API leng_t
CR_FUW(xml_skip_tag) (
  __CR_IN__ const sXNODEx*  nodes,
  __CR_IN__ leng_t          index,
  __CR_IN__ leng_t          count
    )
{
    XCHAR*      name;
    leng_t      loop;
    sXNODEx*    node;

    if (index >= count)
        return (index);
    if (nodes[index].closed)
        return (index + 1);     /* 封闭的标签直接返回下一个 */
    name = nodes[index].name;

    /* 结束标签直接出错返回 */
    if (name[0] == CR_XC('/'))
        return (count);

    /* 遍历所有的 XML 标签, 直到找到下一个同层标签 */
    for (loop = index + 1; loop < count; loop++) {
        node = (sXNODEx*)(&nodes[loop]);
        if (node->name[0] == CR_XC('/')) {
            if (CR_FAW(str_cmpI)(name, &node->name[1]) != 0)
                return (count);
            return (loop + 1);
        }

        /* 递归处理这个非指定的标签 */
        loop = CR_FUW(xml_skip_tag)(nodes, loop, count) - 1;
    }
    return ((leng_t)loop);
}

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
