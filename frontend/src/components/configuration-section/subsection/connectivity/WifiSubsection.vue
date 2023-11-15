<script setup>
import {nextTick, reactive, ref} from "vue";
import WifiNetworkLogin from "../../../modal/WifiNetworkLogin.vue";
import {getWifiStatus, rssiToBootstrapBackground} from "../../../../utils/wifi";
import {Modal} from 'bootstrap'
import {toggleWifiEndpoint} from "../../../../axios/endpoints";
import Spinner from "../../../Spinner.vue";

const wifiEnabled = ref(false);
// Stores state so that the modal knows which network to display
const selectedNetwork = ref('');
// Info about the current connected network
const connectedNetwork = reactive({});
const networks = reactive([]);

const loading = ref(false);


function init() {
  // Clear the networks list
  networks.splice(0, networks.length);
  loading.value = true;
  getWifiStatus((_wifiEnabled, _network, _networks) => {
    loading.value = false;
    wifiEnabled.value = _wifiEnabled;
    connectedNetwork.value = _network;
    networks.push(..._networks);
  });
}

function openNetworkModal(network) {
  selectedNetwork.value = network;
  nextTick(() => new Modal(document.getElementById('wifiNetworkModal')).show());
}

function changeWifiState() {
  toggleWifiEndpoint.post().then(response => {
    wifiEnabled.value = response.data.newState;
  });
}

function refresh() {

}

init();
</script>

<template>
  <div class="row">
    <div class="col">
      <div class="form-check form-switch mt-2">
        <label class="form-check-label" for="wifiState">Enable Wifi</label>
        <input class="form-check-input" type="checkbox" id="wifiState" :checked="wifiEnabled" @click="changeWifiState">
      </div>
    </div>
    <div v-if="wifiEnabled" class="col text-end">
      <button @click="refresh" class="btn btn-outline-secondary" :disabled="true">
        <Icon name="hi-refresh"/>
        Refresh networks
      </button>
    </div>
  </div>

  <section>
    <Spinner v-if="loading"/>
    <ul v-if="wifiEnabled" class="list-group mt-3">
      <li v-for="network in networks" :key="network.mac"
          @click="openNetworkModal(network.mac)"
          class="list-group-item wifi-list-item d-flex justify-content-between"
          :title="`Channel: ${network.channel}, MAC: ${network.mac}`"
      >
        <div>
          <span class="fw-bold d-inline-block me-1">{{ network.ssid }}</span>
          <span class="badge bg-secondary rounded-pill">{{ network.encryption }}</span>
        </div>
        <div>
          <span v-if="network.connected" class="badge rounded-pill bg-primary">{{connectedNetwork.v4_address}}</span>
          <span v-if="network.connected" class="badge rounded-pill align-baseline bg-success mx-1">Connected</span>
          <span class="badge rounded-pill align-baseline"
                :class="rssiToBootstrapBackground(network.rssi)">{{ network.rssi }} dBm</span>
        </div>
      </li>
    </ul>
  </section>
  <WifiNetworkLogin :network="selectedNetwork" :networks="networks" @success="init"/>
</template>

<style scoped>
.wifi-list-item {
  cursor: pointer;
}

.wifi-list-item:hover {
  background-color: #f8f9fa;
}

.spinner-overlay {
  position: absolute;
  z-index: 1000;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);
}

</style>
