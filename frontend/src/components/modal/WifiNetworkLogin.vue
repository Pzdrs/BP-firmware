<script setup>
import {computed, onBeforeUnmount, ref, watchEffect} from "vue";
import {rssiToBootstrapBackground} from "../../utils/wifi";
import {connectToWifiEndpoint, disconnectFromWifiEndpoint} from "../../axios/endpoints";


const props = defineProps({
  network: String,
  networks: Object
});

const emit = defineEmits(['success']);

const loading = ref(false);

const network = computed(() => {
  return props.networks.find(_network => _network.mac === props.network);
});

const networkSelected = computed(() => {
  return !!network.value;
});

const actionButtonDisabled = computed(() => {
  return loading.value;
});

function handleSubmit(event) {
  loading.value = true;
  if (network.value.connected) {
    disconnectFromWifiEndpoint.post().then(() => {
      loading.value = false;
      alert(`Disconnected from ${network.value.ssid} network`);
      emit('success');
    });
  } else {
    const ssid = network.value.ssid;
    const password = event.target.password.value;

    const formData = new URLSearchParams();

    formData.append('ssid', ssid);
    formData.append('password', password);

    connectToWifiEndpoint.post(formData)
        .then(() => {
          alert(`Connected to ${ssid} network`);
          emit('success');
        })
        .catch(() => {
          alert(`Couldn't connect to ${ssid} network`);
        })
        .finally(() => {
          event.target.password.value = '';
          loading.value = false;
        });
  }
}
</script>

<template>
  <div class="modal fade" id="wifiNetworkModal" tabindex="-1" aria-hidden="true"
       v-if="networkSelected">
    <form @submit.prevent="handleSubmit">
      <div class="modal-dialog">
        <div class="modal-content">
          <div class="modal-header">
            <h5 class="modal-title">
              {{ network.ssid }}
              <span class="badge bg-secondary rounded-pill mx-1">{{ network.encryption }}</span>
              <span class="badge rounded-pill align-baseline bg-primary" title="Network channel">{{
                  network.channel
                }}</span>
            </h5>
            <div>
              <span class="badge rounded-pill align-baseline align-top"
                    :class="rssiToBootstrapBackground(network.rssi)">{{ network.rssi }} dBm</span>
              <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
            </div>
          </div>
          <div class="modal-body">
            <section v-if="network.connected">
              <p>
                You're connected to this network
              </p>
            </section>
            <section v-else>
              <div class="row">
                <div class="col">
                  <input type="password" name="password" class="form-control" placeholder="Password" required>
                </div>
              </div>
            </section>
          </div>
          <div class="modal-footer">
            <section v-if="network.connected">
              <button type="submit" class="btn btn-danger" :disabled="actionButtonDisabled">
               <span v-if="loading">
                  <span class="spinner-border spinner-border-sm" role="status" aria-hidden="true"></span>
                  <span class="visually-hidden">Loading...</span>
               </span>
                Disconnect
              </button>
            </section>
            <section v-else>
              <button type="submit" class="btn btn-primary" :disabled="actionButtonDisabled">
                <span v-if="loading">
                  <span class="spinner-border spinner-border-sm" role="status" aria-hidden="true"></span>
                  <span class="visually-hidden">Loading...</span>
               </span>
                Connect
              </button>
            </section>
            <button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Close</button>
          </div>
        </div>
      </div>
    </form>
  </div>
</template>

<style scoped>

</style>
