
def get_bps51(smod, fso, bps):
    f_th1 = (256 - fso / (bps * 32.0))
    h_th1 = int(f_th1 + 0.5)
    if h_th1 < 0:
        return
    if h_th1 == 256:
        if smod == 1:
            return
        get_bps51(1, fso * 2, bps)
        return
    x_bps = fso / 32.0 / (256 - h_th1)
    delta = ((x_bps - bps) / bps) * 100.0
    if delta > 2.0 or delta < -2.0:
        if smod == 1:
            return
        get_bps51(1, fso * 2, bps)
        return
    display = "flt=%5.4f, hex=0x%02X, smod=%u, bps=%u, delta=%3.2f%%" \
                %(f_th1, h_th1, smod, bps, delta)
    print display

fso = float(raw_input())
get_bps51(0, fso, 110.0)
get_bps51(0, fso, 300.0)
get_bps51(0, fso, 600.0)
get_bps51(0, fso, 1200.0)
get_bps51(0, fso, 2400.0)
get_bps51(0, fso, 4800.0)
get_bps51(0, fso, 9600.0)
get_bps51(0, fso, 14400.0)
get_bps51(0, fso, 19200.0)
get_bps51(0, fso, 38400.0)
get_bps51(0, fso, 57600.0)
get_bps51(0, fso, 115200.0)
get_bps51(0, fso, 128000.0)
get_bps51(0, fso, 256000.0)
