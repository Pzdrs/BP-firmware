<script setup>

import {mqttConfigEndpoint, mqttTestEndpoint} from "../../../../axios/endpoints";
import {ref} from "vue";

const DEFAULT_MQTT_PORT = 1883;

const testing = ref(false);
const dryRun = ref(false);
const saving = ref(false);

const alerts = ref([]);

function handleSubmit(event) {
  if (dryRun.value) {
    testConnection();
    dryRun.value = false;
    return;
  }
  
  saving.value = true;
  alerts.value = [];

  const username = event.target.username.value;
  const password = event.target.password.value;
  const server = event.target.serverAddress.value;
  const port = event.target.port.value || DEFAULT_MQTT_PORT;

  mqttConfigEndpoint.post({
    username, password, server, port
  }).then(() => {
    alerts.value = [{
      message: "The configuration was saved successfully.",
      class: "alert-success"
    }];
  }).catch(() => {
    alerts.value = [{
      message: "The configuration couldn't be saved.",
      class: "alert-danger"
    }];
  }).finally(() => {
    saving.value = false;
  });
}

function testConnection() {
  testing.value = true;
  alerts.value = [];
  mqttTestEndpoint.get()
      .then(response => {
        const result = response.data.success;
        testing.value = false;
        if (result) {
          alerts.value = [{
            message: "The connection was successful.",
            class: "alert-success"
          }];
        } else {
          alerts.value = [{
            message: "The connection failed.",
            class: "alert-danger"
          }];
        }
      });
}
</script>

<template>
  <div v-for="(alert, index) in alerts" :key="index" class="alert" :class="alert.class" role="alert">
    {{ alert.message }}
  </div>
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
      <button type="submit" class="btn btn-info me-2" @click="dryRun = true">
        <span v-if="testing">
          <span class="spinner-border spinner-border-sm" role="status" aria-hidden="true"></span>
          <span class="visually-hidden">Loading...</span>
        </span>
        Test
      </button>
      <button type="submit" class="btn btn-primary">Save changes</button>
    </div>
  </form>
</template>

<style scoped>

</style>
