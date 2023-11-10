import EndpointFactory from "axios-endpoints";
import espApi from "./index";

const Endpoint = EndpointFactory(espApi);

export const scanWifiNetworks = new Endpoint('/wifi/scan');
