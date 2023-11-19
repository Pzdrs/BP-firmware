import EndpointFactory from "axios-endpoints";
import espApi from "./index";

const Endpoint = EndpointFactory(espApi);

export const statusEndpoint = new Endpoint('/status');

export const mqttConfigEndpoint = new Endpoint('/mqtt');
export const mqttTestEndpoint = new Endpoint('/mqtt/test');

export const wifiEndpoint = new Endpoint('/wifi');
export const toggleWifiEndpoint = new Endpoint('/wifi/toggle-state');
export const connectToWifiEndpoint = new Endpoint('/wifi/connect');
export const disconnectFromWifiEndpoint = new Endpoint('/wifi/disconnect');
