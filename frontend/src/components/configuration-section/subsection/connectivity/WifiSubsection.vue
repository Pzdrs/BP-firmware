<script setup>
import {nextTick, onMounted, reactive, ref} from "vue";
import WifiNetworkLogin from "../../../modal/WifiNetworkLogin.vue";
import {rssiToBootstrapBackground} from "../../../../utils/wifi";
import {Modal} from 'bootstrap'

const currentNetwork = ref('');

const wifiEnabled = ref(true);

const networks = reactive([
  {
    ssid: 'IOT',
    security: 'WPA2',
    rssi: -35,
    connected: true
  },
  {
    ssid: 'Guest',
    security: 'WPA2',
    rssi: -75,
    connected: false
  },
  {
    ssid: 'Home Wifi',
    security: 'WPA2',
    rssi: -96,
    connected: false
  }
]);

function setCurrentNetwork(network) {
  currentNetwork.value = network;
  nextTick(() => new Modal(document.getElementById('wifiNetworkModal')).show());
}

function refresh() {
  console.log('refresh');
}

function changeWifiState() {
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
      <button @click="refresh" class="btn btn-outline-secondary">
        <Icon name="hi-refresh"/>
        Refresh networks
      </button>
    </div>
  </div>
  <ul v-if="wifiEnabled" class="list-group mt-3">
    <li v-for="network in networks" :key="network.ssid"
        @click="setCurrentNetwork(network.ssid)"
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
  <WifiNetworkLogin :network="currentNetwork" :networks="networks"/>
</template>

<style scoped>
.wifi-list-item {
  cursor: pointer;
}

.wifi-list-item:hover {
  background-color: #f8f9fa;
}
</style>
