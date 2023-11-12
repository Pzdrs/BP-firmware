<script setup>

import {ref} from "vue";

const DEFAULT_MQTT_PORT = 1883;

const testing = ref(false);
const testingSuccessful = ref(false);


function handleSubmit(event) {
  const username = event.target.username.value;
  const password = event.target.password.value;
  const serverAddress = event.target.serverAddress.value;
  const serverPort = event.target.port.value || DEFAULT_MQTT_PORT;
}

function testConnection() {
  testing.value = true;
  setTimeout(() => {
    testingSuccessful.value = true;
    testing.value = false;
  }, 1000);
}
</script>

<template>
  <form @submit.prevent="handleSubmit">
    <div class="row mb-3">
      <div class="col">
        <input type="text" name="username" class="form-control" placeholder="Username">
      </div>
      <div class="col">
        <input type="password" name="password" class="form-control" placeholder="Password">
      </div>
    </div>
    <div class="row mb-3">
      <div class="col">
        <div class="input-group">
          <span class="input-group-text">Broker settings</span>
          <input type="text" name="serverAddress" class="form-control" placeholder="Server address" required>
          <input type="number" name="port" class="form-control" :placeholder="DEFAULT_MQTT_PORT">
        </div>
      </div>
    </div>
    <div class="text-end">
      <button class="btn btn-info me-2" @click="testConnection">
        <span v-if="testing">
          <span class="spinner-border spinner-border-sm" role="status" aria-hidden="true"></span>
          <span class="visually-hidden">Loading...</span>
        </span>
        <Icon v-if="testingSuccessful" name="fc-checkmark"/>
        Test
      </button>
      <button type="submit" class="btn btn-primary">Save changes</button>
    </div>
  </form>
</template>

<style scoped>

</style>
