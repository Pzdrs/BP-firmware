import EndpointFactory from "axios-endpoints";
import espApi from "./index";

const Endpoint = EndpointFactory(espApi);

export const mqttConfigEndpoint = new Endpoint('/mqtt');

export const wifiEndpoint = new Endpoint('/wifi');
export const toggleWifiEndpoint = new Endpoint('/wifi/toggle-state');
export const discoverWifiNetworksEndpoint = new Endpoint('/wifi/refresh');
export const connectToWifiEndpoint = new Endpoint('/wifi/connect');
export const disconnectFromWifiEndpoint = new Endpoint('/wifi/disconnect');
