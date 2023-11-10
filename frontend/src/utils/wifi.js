import {scanWifiNetworks} from "../axios/endpoints";

const SECURITY_MODES = [
    'OPEN',
    'WEP',
    'WPA_PSK',
    'WPA2_PSK',
    'WPA_WPA2_PSK',
    'WPA2_ENTERPRISE',
    'WPA3_PSK',
    'WPA2_WPA3_PSK',
    'WAPI_PSK',
    'MAX'
];


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

function filterNetworks(networks) {
    return networks.filter((network) => {
        return true;
    });
}

function transformNetwork(network) {
    return {
        ssid: network['ssid'],
        rssi: network['rssi'],
        mac: network['bssid'],
        security: SECURITY_MODES[network['secure']],
        channel: network['channel'],
        connected: false
    }
}

export function getAvailableNetworks(callback) {
    scanWifiNetworks.get().then((response) => {
        const networks = response.data.map(transformNetwork);
        callback(filterNetworks(networks));
    });
}
