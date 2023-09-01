import numpy as np
import pandas as pd

def read_pdg_data(filename):
    """read the cross section data from PDG."""
    start_data = False
    meta_keys = []
    meta_values = []
    data_formats = None

    # keep track of meta data lines
    meta_info_lines = 4
    ilno = -1
    default_headers = " FILE_NAME\n REACTION\n BEAM_MASS TARGET_MASS THRESHOLD FINAL_STATE_MULTIPLICITY\n NUMBER_OF_DATA_POINTS\n POINT_NUMBER PLAB(GEV/C) PLAB_MIN PLAB_MAX SIG(MB) STA_ERR+ STA_ERR- SY_ER+(PCT) SY_ER-(PCT) REFERENCE FLAG\n  FORMAT(I5,1X,4F11.5,2F8.4,1X,2F6.1,A)\n"
    data_headers = ["idx", "plab", "plab_min", "plab_max", "sig", "stats_up", "stats_down", "sys_up", "sys_down"]

    headers = ""
    data = []
    with open(filename, 'r') as f:
        for line in f:
            if "---" in line:
                assert headers == default_headers, "headers should be the same as the default"
                start_data = True
                continue

            if not start_data:
                headers += line
                if "FORMAT" in line:
                    data_formats = line.replace("FORMAT(","").replace(")", "").split(",")
                elif "POINT_NUMBER" in line:
                    # data_headers = line[:-1].split()
                    pass
                else:
                    meta_keys += line[:-1].split()
            else:
                ilno += 1
                if ilno < meta_info_lines:
                    if ilno < 2:
                        meta_values.append(line[:-1])
                    else:
                        meta_values += line[:-1].split()
                else:
                    # Convert elements to the appropriate data types
                    elements = line[:-1].split()
                    point_number = int(elements[0])
                    plab = float(elements[1])
                    plab_min = float(elements[2])
                    plab_max = float(elements[3])
                    sig_mb = float(elements[4])
                    sta_err_plus = float(elements[5])
                    sta_err_minus = float(elements[6])
                    sy_er_pct_plus = float(elements[7])
                    sy_er_pct_minus = float(elements[8])
                    data.append([point_number, plab, plab_min, plab_max, sig_mb, sta_err_plus, sta_err_minus, sy_er_pct_plus, sy_er_pct_minus])

    df = pd.DataFrame(data, columns=data_headers)
    error_up = np.sqrt(df.stats_up**2 + df.sys_up**2)
    error_down = np.sqrt(df.stats_down**2 + df.sys_down**2)
    x_up = df.plab_max - df.plab
    x_down = df.plab - df.plab_min
    df = df.assign(error_up=error_up, error_down=error_down, x_up=x_up, x_down=x_down)
    meta_data = dict(zip(meta_keys, meta_values))
    
    return df, meta_data
