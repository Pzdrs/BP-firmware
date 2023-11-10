<script setup>
import {computed} from "vue";
import {rssiToBootstrapBackground} from "../../utils/wifi";

const props = defineProps({
  network: String,
  networks: Object
});

const network = computed(() => {
  return props.networks.find(_network => _network.ssid === props.network);
});

const networkSelected = computed(() => {
  return !!network.value;
});

function handleSubmit(event) {
  if (network.value.connected) {

  } else {
    const username = event.target.username.value;
    const password = event.target.password.value;
    console.log(username, password);
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
              <span class="badge bg-secondary rounded-pill mx-1">{{ network.security }}</span>
              <span class="badge rounded-pill align-baseline bg-primary" title="Network channel">{{ network.channel }}</span>
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
              <div class="row mb-3">
                <div class="col">
                  <input type="text" name="username" class="form-control" placeholder="Username" required>
                </div>
                <div class="col">
                  <input type="password" name="password" class="form-control" placeholder="Password" required>
                </div>
              </div>
            </section>
          </div>
          <div class="modal-footer">
            <section v-if="network.connected">
              <button type="submit" class="btn btn-danger">Disconnect</button>
            </section>
            <section v-else>
              <button type="submit" class="btn btn-primary">Connect</button>
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
