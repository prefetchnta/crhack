/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2014-03-20  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack HTML 相关操作函数库模板 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#if !defined(_CR_NO_WIDE_FUNC_)

/* HTML 转义查表结构 */
typedef struct
{
        int32u          ucs4;
        const XCHAR*    name;

} sHTML_CVT;

/* HTML 转义字符常数表 */
static const sHTML_CVT _rom_ s_html_cvt[] =
{
    {   0x0022UL, CR_XS("quot")         },
    {   0x0026UL, CR_XS("amp")          },
    {   0x0027UL, CR_XS("apos")         },
    {   0x003CUL, CR_XS("lt")           },
    {   0x003EUL, CR_XS("gt")           },
    {   0x00A0UL, CR_XS("nbsp")         },
    {   0x00A1UL, CR_XS("iexcl")        },
    {   0x00A2UL, CR_XS("cent")         },
    {   0x00A3UL, CR_XS("pound")        },
    {   0x00A4UL, CR_XS("curren")       },
    {   0x00A5UL, CR_XS("yen")          },
    {   0x00A6UL, CR_XS("brvbar")       },
    {   0x00A7UL, CR_XS("sect")         },
    {   0x00A8UL, CR_XS("uml")          },
    {   0x00A9UL, CR_XS("copy")         },
    {   0x00AAUL, CR_XS("ordf")         },
    {   0x00ABUL, CR_XS("laquo")        },
    {   0x00ACUL, CR_XS("not")          },
    {   0x00ADUL, CR_XS("shy")          },
    {   0x00AEUL, CR_XS("reg")          },
    {   0x00AFUL, CR_XS("macr")         },
    {   0x00B0UL, CR_XS("deg")          },
    {   0x00B1UL, CR_XS("plusmn")       },
    {   0x00B2UL, CR_XS("sup2")         },
    {   0x00B3UL, CR_XS("sup3")         },
    {   0x00B4UL, CR_XS("acute")        },
    {   0x00B5UL, CR_XS("micro")        },
    {   0x00B6UL, CR_XS("para")         },
    {   0x00B7UL, CR_XS("middot")       },
    {   0x00B8UL, CR_XS("cedil")        },
    {   0x00B9UL, CR_XS("sup1")         },
    {   0x00BAUL, CR_XS("ordm")         },
    {   0x00BBUL, CR_XS("raquo")        },
    {   0x00BCUL, CR_XS("frac14")       },
    {   0x00BDUL, CR_XS("frac12")       },
    {   0x00BEUL, CR_XS("frac34")       },
    {   0x00BFUL, CR_XS("iquest")       },
    {   0x00C0UL, CR_XS("Agrave")       },
    {   0x00C1UL, CR_XS("Aacute")       },
    {   0x00C2UL, CR_XS("Acirc")        },
    {   0x00C3UL, CR_XS("Atilde")       },
    {   0x00C4UL, CR_XS("Auml")         },
    {   0x00C5UL, CR_XS("Aring")        },
    {   0x00C6UL, CR_XS("AElig")        },
    {   0x00C7UL, CR_XS("Ccedil")       },
    {   0x00C8UL, CR_XS("Egrave")       },
    {   0x00C9UL, CR_XS("Eacute")       },
    {   0x00CAUL, CR_XS("Ecirc")        },
    {   0x00CBUL, CR_XS("Euml")         },
    {   0x00CCUL, CR_XS("Igrave")       },
    {   0x00CDUL, CR_XS("Iacute")       },
    {   0x00CEUL, CR_XS("Icirc")        },
    {   0x00CFUL, CR_XS("Iuml")         },
    {   0x00D0UL, CR_XS("ETH")          },
    {   0x00D1UL, CR_XS("Ntilde")       },
    {   0x00D2UL, CR_XS("Ograve")       },
    {   0x00D3UL, CR_XS("Oacute")       },
    {   0x00D4UL, CR_XS("Ocirc")        },
    {   0x00D5UL, CR_XS("Otilde")       },
    {   0x00D6UL, CR_XS("Ouml")         },
    {   0x00D7UL, CR_XS("times")        },
    {   0x00D8UL, CR_XS("Oslash")       },
    {   0x00D9UL, CR_XS("Ugrave")       },
    {   0x00DAUL, CR_XS("Uacute")       },
    {   0x00DBUL, CR_XS("Ucirc")        },
    {   0x00DCUL, CR_XS("Uuml")         },
    {   0x00DDUL, CR_XS("Yacute")       },
    {   0x00DEUL, CR_XS("THORN")        },
    {   0x00DFUL, CR_XS("szlig")        },
    {   0x00E0UL, CR_XS("agrave")       },
    {   0x00E1UL, CR_XS("aacute")       },
    {   0x00E2UL, CR_XS("acirc")        },
    {   0x00E3UL, CR_XS("atilde")       },
    {   0x00E4UL, CR_XS("auml")         },
    {   0x00E5UL, CR_XS("aring")        },
    {   0x00E6UL, CR_XS("aelig")        },
    {   0x00E7UL, CR_XS("ccedil")       },
    {   0x00E8UL, CR_XS("egrave")       },
    {   0x00E9UL, CR_XS("eacute")       },
    {   0x00EAUL, CR_XS("ecirc")        },
    {   0x00EBUL, CR_XS("euml")         },
    {   0x00ECUL, CR_XS("igrave")       },
    {   0x00EDUL, CR_XS("iacute")       },
    {   0x00EEUL, CR_XS("icirc")        },
    {   0x00EFUL, CR_XS("iuml")         },
    {   0x00F0UL, CR_XS("eth")          },
    {   0x00F1UL, CR_XS("ntilde")       },
    {   0x00F2UL, CR_XS("ograve")       },
    {   0x00F3UL, CR_XS("oacute")       },
    {   0x00F4UL, CR_XS("ocirc")        },
    {   0x00F5UL, CR_XS("otilde")       },
    {   0x00F6UL, CR_XS("ouml")         },
    {   0x00F7UL, CR_XS("divide")       },
    {   0x00F8UL, CR_XS("oslash")       },
    {   0x00F9UL, CR_XS("ugrave")       },
    {   0x00FAUL, CR_XS("uacute")       },
    {   0x00FBUL, CR_XS("ucirc")        },
    {   0x00FCUL, CR_XS("uuml")         },
    {   0x00FDUL, CR_XS("yacute")       },
    {   0x00FEUL, CR_XS("thorn")        },
    {   0x00FFUL, CR_XS("yuml")         },
    {   0x0152UL, CR_XS("OElig")        },
    {   0x0153UL, CR_XS("oelig")        },
    {   0x0160UL, CR_XS("Scaron")       },
    {   0x0161UL, CR_XS("scaron")       },
    {   0x0178UL, CR_XS("Yuml")         },
    {   0x0192UL, CR_XS("fnof")         },
    {   0x02C6UL, CR_XS("circ")         },
    {   0x02DCUL, CR_XS("tilde")        },
    {   0x0391UL, CR_XS("Alpha")        },
    {   0x0392UL, CR_XS("Beta")         },
    {   0x0393UL, CR_XS("Gamma")        },
    {   0x0394UL, CR_XS("Delta")        },
    {   0x0395UL, CR_XS("Epsilon")      },
    {   0x0396UL, CR_XS("Zeta")         },
    {   0x0397UL, CR_XS("Eta")          },
    {   0x0398UL, CR_XS("Theta")        },
    {   0x0399UL, CR_XS("Iota")         },
    {   0x039AUL, CR_XS("Kappa")        },
    {   0x039BUL, CR_XS("Lambda")       },
    {   0x039CUL, CR_XS("Mu")           },
    {   0x039DUL, CR_XS("Nu")           },
    {   0x039EUL, CR_XS("Xi")           },
    {   0x039FUL, CR_XS("Omicron")      },
    {   0x03A0UL, CR_XS("Pi")           },
    {   0x03A1UL, CR_XS("Rho")          },
    {   0x03A3UL, CR_XS("Sigma")        },
    {   0x03A4UL, CR_XS("Tau")          },
    {   0x03A5UL, CR_XS("Upsilon")      },
    {   0x03A6UL, CR_XS("Phi")          },
    {   0x03A7UL, CR_XS("Chi")          },
    {   0x03A8UL, CR_XS("Psi")          },
    {   0x03A9UL, CR_XS("Omega")        },
    {   0x03B1UL, CR_XS("alpha")        },
    {   0x03B2UL, CR_XS("beta")         },
    {   0x03B3UL, CR_XS("gamma")        },
    {   0x03B4UL, CR_XS("delta")        },
    {   0x03B5UL, CR_XS("epsilon")      },
    {   0x03B6UL, CR_XS("zeta")         },
    {   0x03B7UL, CR_XS("eta")          },
    {   0x03B8UL, CR_XS("theta")        },
    {   0x03B9UL, CR_XS("iota")         },
    {   0x03BAUL, CR_XS("kappa")        },
    {   0x03BBUL, CR_XS("lambda")       },
    {   0x03BCUL, CR_XS("mu")           },
    {   0x03BDUL, CR_XS("nu")           },
    {   0x03BEUL, CR_XS("xi")           },
    {   0x03BFUL, CR_XS("omicron")      },
    {   0x03C0UL, CR_XS("pi")           },
    {   0x03C1UL, CR_XS("rho")          },
    {   0x03C2UL, CR_XS("sigmaf")       },
    {   0x03C3UL, CR_XS("sigma")        },
    {   0x03C4UL, CR_XS("tau")          },
    {   0x03C5UL, CR_XS("upsilon")      },
    {   0x03C6UL, CR_XS("phi")          },
    {   0x03C7UL, CR_XS("chi")          },
    {   0x03C8UL, CR_XS("psi")          },
    {   0x03C9UL, CR_XS("omega")        },
    {   0x03D1UL, CR_XS("thetasym")     },
    {   0x03D2UL, CR_XS("upsih")        },
    {   0x03D6UL, CR_XS("piv")          },
    {   0x2002UL, CR_XS("ensp")         },
    {   0x2003UL, CR_XS("emsp")         },
    {   0x2009UL, CR_XS("thinsp")       },
    {   0x200CUL, CR_XS("zwnj")         },
    {   0x200DUL, CR_XS("zwj")          },
    {   0x200EUL, CR_XS("lrm")          },
    {   0x200FUL, CR_XS("rlm")          },
    {   0x2013UL, CR_XS("ndash")        },
    {   0x2014UL, CR_XS("mdash")        },
    {   0x2018UL, CR_XS("lsquo")        },
    {   0x2019UL, CR_XS("rsquo")        },
    {   0x201AUL, CR_XS("sbquo")        },
    {   0x201CUL, CR_XS("ldquo")        },
    {   0x201DUL, CR_XS("rdquo")        },
    {   0x201EUL, CR_XS("bdquo")        },
    {   0x2020UL, CR_XS("dagger")       },
    {   0x2021UL, CR_XS("Dagger")       },
    {   0x2022UL, CR_XS("bull")         },
    {   0x2026UL, CR_XS("hellip")       },
    {   0x2030UL, CR_XS("permil")       },
    {   0x2032UL, CR_XS("prime")        },
    {   0x2033UL, CR_XS("Prime")        },
    {   0x2039UL, CR_XS("lsaquo")       },
    {   0x203AUL, CR_XS("rsaquo")       },
    {   0x203EUL, CR_XS("oline")        },
    {   0x2044UL, CR_XS("frasl")        },
    {   0x20ACUL, CR_XS("euro")         },
    {   0x2111UL, CR_XS("image")        },
    {   0x2118UL, CR_XS("weierp")       },
    {   0x211CUL, CR_XS("real")         },
    {   0x2122UL, CR_XS("trade")        },
    {   0x2135UL, CR_XS("alefsym")      },
    {   0x2190UL, CR_XS("larr")         },
    {   0x2191UL, CR_XS("uarr")         },
    {   0x2192UL, CR_XS("rarr")         },
    {   0x2193UL, CR_XS("darr")         },
    {   0x2194UL, CR_XS("harr")         },
    {   0x21B5UL, CR_XS("crarr")        },
    {   0x21D0UL, CR_XS("lArr")         },
    {   0x21D1UL, CR_XS("uArr")         },
    {   0x21D2UL, CR_XS("rArr")         },
    {   0x21D3UL, CR_XS("dArr")         },
    {   0x21D4UL, CR_XS("hArr")         },
    {   0x2200UL, CR_XS("forall")       },
    {   0x2202UL, CR_XS("part")         },
    {   0x2203UL, CR_XS("exist")        },
    {   0x2205UL, CR_XS("empty")        },
    {   0x2207UL, CR_XS("nabla")        },
    {   0x2208UL, CR_XS("isin")         },
    {   0x2209UL, CR_XS("notin")        },
    {   0x220BUL, CR_XS("ni")           },
    {   0x220FUL, CR_XS("prod")         },
    {   0x2211UL, CR_XS("sum")          },
    {   0x2212UL, CR_XS("minus")        },
    {   0x2217UL, CR_XS("lowast")       },
    {   0x221AUL, CR_XS("radic")        },
    {   0x221DUL, CR_XS("prop")         },
    {   0x221EUL, CR_XS("infin")        },
    {   0x2220UL, CR_XS("ang")          },
    {   0x2227UL, CR_XS("and")          },
    {   0x2228UL, CR_XS("or")           },
    {   0x2229UL, CR_XS("cap")          },
    {   0x222AUL, CR_XS("cup")          },
    {   0x222BUL, CR_XS("int")          },
    {   0x2234UL, CR_XS("there4")       },
    {   0x223CUL, CR_XS("sim")          },
    {   0x2245UL, CR_XS("cong")         },
    {   0x2248UL, CR_XS("asymp")        },
    {   0x2260UL, CR_XS("ne")           },
    {   0x2261UL, CR_XS("equiv")        },
    {   0x2264UL, CR_XS("le")           },
    {   0x2265UL, CR_XS("ge")           },
    {   0x2282UL, CR_XS("sub")          },
    {   0x2283UL, CR_XS("sup")          },
    {   0x2284UL, CR_XS("nsub")         },
    {   0x2286UL, CR_XS("sube")         },
    {   0x2287UL, CR_XS("supe")         },
    {   0x2295UL, CR_XS("oplus")        },
    {   0x2297UL, CR_XS("otimes")       },
    {   0x22A5UL, CR_XS("perp")         },
    {   0x22C5UL, CR_XS("sdot")         },
    {   0x22EEUL, CR_XS("vellip")       },
    {   0x2308UL, CR_XS("lceil")        },
    {   0x2309UL, CR_XS("rceil")        },
    {   0x230AUL, CR_XS("lfloor")       },
    {   0x230BUL, CR_XS("rfloor")       },
    {   0x2329UL, CR_XS("lang")         },
    {   0x232AUL, CR_XS("rang")         },
    {   0x25CAUL, CR_XS("loz")          },
    {   0x2660UL, CR_XS("spades")       },
    {   0x2663UL, CR_XS("clubs")        },
    {   0x2665UL, CR_XS("hearts")       },
    {   0x2666UL, CR_XS("diams")        },
};

/*
=======================================
    HTML 转义字符串转换
=======================================
*/
CR_API XCHAR*
CR_FUW(html_to_string) (
  __CR_IN__ const XCHAR*    str
    )
{
    uint_t  cpy;
    int32u  val;
    leng_t  idx, len, jj, kk;
    XCHAR   cha, *htm, *ptr, tmp[9];

    /* 分配足够多的空间 */
    len = CR_FAW(str_len)(str);
    htm = CR_FAW(str_alloc)(len + 1);
    if (htm == NULL)
        return (NULL);
    ptr = htm;

    /* 开始转换编码 */
    for (idx = 0; idx < len; idx++) {
        cha = *str++;
        if (cha != CR_XC('&')) {
            *ptr++ = cha;
            continue;
        }

        /* 抽取转义后的符号 */
        for (kk = 0; kk < 9; kk++) {
            if (kk + idx >= len)
                goto _failure;
            if (str[kk] == CR_XC(';'))
                break;
            tmp[kk] = str[kk];
        }
        if (kk < 2 || kk >= 9)
            goto _failure;
        tmp[kk] = CR_XN(NIL);
        idx += kk + 1;
        str += kk + 1;

        /* 是否为数字型 */
        if (tmp[0] == CR_XC('#'))
        {
            if (tmp[1] == CR_XC('x')) {
                val = CR_FAW(str2hex)(&tmp[2], &jj);
                kk -= 2;
            }
            else {
                if (!CR_FAW(is_digit)(tmp[1]))
                    goto _failure;
                val = CR_FAW(str2int)(&tmp[1], &jj);
                kk -= 1;
            }
            if (kk <= 1 || jj != kk)
                goto _failure;
        }
        else
        {
            /* 逐个比较名称 */
            for (jj = 0; jj < cntsof(s_html_cvt); jj++) {
                if (CR_FAW(str_cmp)(tmp, s_html_cvt[jj].name) == 0)
                    break;
            }
            if (jj >= cntsof(s_html_cvt))
                goto _failure;
            val = s_html_cvt[jj].ucs4;
        }

        /* 输出目标 UNICODE 值 */
#if defined(_CR_BUILD_ANSI_)
        cpy = ucs4_to_utf8(ptr, val);
#else
        cpy = ucs4_to_utf16(ptr, val);
#endif
        if (cpy == 0)
            goto _failure;
        ptr = (XCHAR*)((byte_t*)ptr + cpy);
    }
    *ptr++ = CR_XN(NIL);
    len = (leng_t)(ptr - htm);

    /* 重新分配字符串 */
    ptr = CR_FAW(str_alloc)(len);
    if (ptr == NULL)
        goto _failure;
    CR_FAW(chr_cpy)(ptr, htm, len);
    mem_free(htm);
    return (ptr);

_failure:
    mem_free(htm);
    return (NULL);
}

#endif  /* !_CR_NO_WIDE_FUNC_ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
