const BAD_RSSI = -80;
const GOOD_RSSI = -55;

const COLORS = {
    1: ['#c80000', 'bg-danger'],
    2: ['#dcc800', 'bg-warning'],
    3: ['#00c800', 'bg-success']
};


/**
 * Returns 1, 2 o 3 depending on how good the signal is (the higher the better)
 */
export function normalizeRSSI(rssi) {
    if (rssi <= BAD_RSSI) {
        return 1;
    } else if (rssi >= GOOD_RSSI) {
        return 3;
    } else return 2;
}

export function rssiToColor(rssi) {
    const level = normalizeRSSI(rssi);
    return COLORS[level][0];
}

export function rssiToBootstrapBackground(rssi) {
    const level = normalizeRSSI(rssi);
    return COLORS[level][1];
}
