import {wifiEndpoint} from "../axios/endpoints";

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
    // Sort networks by SSID and then by RSSI in descending order
    networks.sort((a, b) => {
        if (a.ssid !== b.ssid) {
            return a.ssid.localeCompare(b.ssid);
        } else {
            return b.rssi - a.rssi;
        }
    });

    // Filter out networks with lower RSSI for each unique SSID
    const uniqueNetworks = [];
    for (const network of networks) {
        if (network.connected || !uniqueNetworks.some((unique) => unique.ssid === network.ssid)) {
            uniqueNetworks.push(network);
        }
    }

    return uniqueNetworks;
}

function transformNetwork(network) {
    return {
        ssid: network['ssid'],
        rssi: network['rssi'],
        mac: network['bssid'],
        encryption: SECURITY_MODES[network['encryption']],
        channel: network['channel'],
        connected: network['connected']
    }
}

export function getWifiStatus(callback) {
    wifiEndpoint.get().then((response) => {
        const wifiEnabled = response.data.enabled;
        const connectedNetwork = response.data.network;
        // Sorting (the current connected network goes first, then descending RSSI)
        const sortedNetworks = filterNetworks(response.data.networks.map(transformNetwork))
            .sort((a, b) => {
                if (a.connected !== b.connected) {
                    return b.connected - a.connected;
                } else {
                    return b.rssi - a.rssi;
                }
            });
        callback(wifiEnabled, connectedNetwork, sortedNetworks);
    });
}
