# macro noms des objets
OBJETS = \
 lbccodec.o coder.o decod.o lpc.o exc_lbc.o\
 lsp.o tab_lbc.o util_lbc.o basop.o vad.o\
 cod_cng.o dec_cng.o util_cng.o tame.o

# edition de liens
lbccodec : $(OBJETS)
	cc -o lbccodec $(OBJETS) -lm

# Compilations si changements

lbccodec.o : lbccodec.c typedef.h basop.h cst_lbc.h\
    tab_lbc.h lbccodec.h coder.h decod.h exc_lbc.h\
    util_lbc.h cod_cng.h dec_cng.h vad.h
	cc -c -std lbccodec.c

coder.o : coder.c typedef.h basop.h cst_lbc.h\
    tab_lbc.h coder.h lpc.h lsp.h exc_lbc.h\
    util_lbc.h vad.h cod_cng.h tame.h
	cc -c -std coder.c

decod.o : decod.c typedef.h basop.h cst_lbc.h\
    tab_lbc.h lbccodec.h decod.h util_lbc.h lpc.h\
    lsp.h exc_lbc.h dec_cng.h
	cc -c -std decod.c

lpc.o : lpc.c typedef.h basop.h cst_lbc.h\
    tab_lbc.h lbccodec.h coder.h decod.h util_lbc.h\
    lpc.h cod_cng.h
	cc -c -std lpc.c

exc_lbc.o : exc_lbc.c typedef.h basop.h cst_lbc.h\
    tab_lbc.h lbccodec.h util_lbc.h exc_lbc.h tame.h\
    util_cng.h
	cc -c -std exc_lbc.c

lsp.o : lsp.c typedef.h basop.h cst_lbc.h\
    tab_lbc.h lsp.h
	cc -c -std lsp.c

tab_lbc.o : tab_lbc.c typedef.h cst_lbc.h tab_lbc.h
	cc -c -std tab_lbc.c

util_lbc.o : util_lbc.c typedef.h basop.h cst_lbc.h\
    tab_lbc.h lbccodec.h coder.h decod.h util_lbc.h
	cc -c -std util_lbc.c

basop.o : basop.c typedef.h basop.h
	cc -c -std basop.c

vad.o : vad.c typedef.h basop.h cst_lbc.h\
    tab_lbc.h lsp.h vad.h coder.h lbccodec.h
	cc -c -std vad.c

cod_cng.o : cod_cng.c typedef.h cst_lbc.h tab_lbc.h\
    util_lbc.h basop.h lsp.h lpc.h util_cng.h\
    cod_cng.h vad.h coder.h
	cc -c -std cod_cng.c

dec_cng.o : dec_cng.c typedef.h cst_lbc.h tab_lbc.h\
    util_lbc.h lsp.h exc_lbc.h basop.h util_cng.h\
    dec_cng.h decod.h
	cc -c -std dec_cng.c

util_cng.o : util_cng.c typedef.h cst_lbc.h tab_lbc.h\
    util_lbc.h exc_lbc.h basop.h util_cng.h
	cc -c -std util_cng.c

tame.o : tame.c typedef.h basop.h cst_lbc.h\
    lbccodec.h coder.h exc_lbc.h tame.h tab_lbc.h
	cc -c -std tame.c
