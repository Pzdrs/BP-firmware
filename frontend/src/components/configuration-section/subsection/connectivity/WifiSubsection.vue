<script setup>
import {nextTick, reactive, ref} from "vue";
import WifiNetworkLogin from "../../../modal/WifiNetworkLogin.vue";
import {getAvailableNetworks, rssiToBootstrapBackground} from "../../../../utils/wifi";
import {Modal} from 'bootstrap'

const wifiEnabled = ref(true);
const currentNetwork = ref('');
const networks = reactive([]);

const loading = ref(false);


function scanNetworks() {
  // Clear the networks list
  networks.splice(0, networks.length);
  loading.value = true;
  getAvailableNetworks((_networks) => {
    loading.value = false;
    networks.push(..._networks);
  });
}

function openNetworkModal(network) {
  currentNetwork.value = network;
  nextTick(() => new Modal(document.getElementById('wifiNetworkModal')).show());
}

function changeWifiState() {
  // If turning the Wi-Fi on for the first time, automatically scan networks
  if (!wifiEnabled.value && networks.length === 0) scanNetworks();
  wifiEnabled.value = !wifiEnabled.value;
}

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
      <button @click="scanNetworks" class="btn btn-outline-secondary" :disabled="loading">
        <Icon name="hi-refresh"/>
        Refresh networks
      </button>
    </div>
  </div>

  <section>
    <div class="text-center spinner-overlay" v-if="loading">
      <div class="spinner-border" role="status">
        <span class="visually-hidden">Loading...</span>
      </div>
    </div>

    <ul v-if="wifiEnabled" class="list-group mt-3">
      <li v-for="network in networks" :key="network.ssid"
          @click="openNetworkModal(network.ssid)"
          class="list-group-item wifi-list-item d-flex justify-content-between">
        <div>
          <span class="fw-bold d-inline-block me-1">{{ network.ssid }}</span>
          <span class="badge bg-secondary rounded-pill">{{ network.security }}</span>
        </div>
        <div>
          <span v-if="network.connected" class="badge rounded-pill align-baseline bg-success me-1">Connected</span>
          <span class="badge rounded-pill align-baseline"
                :class="rssiToBootstrapBackground(network.rssi)">{{ network.rssi }} dBm</span>
        </div>
      </li>
    </ul>
  </section>
  <WifiNetworkLogin :network="currentNetwork" :networks="networks"/>
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
