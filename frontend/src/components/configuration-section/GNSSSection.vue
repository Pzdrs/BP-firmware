<script setup>
import {ref} from "vue";

const positionFixed = ref(true);
const longitude = ref(0);
const latitude = ref(0);
const altitude = ref(0);

const socket = new WebSocket(`ws://${window.location.hostname}/gnss`);
socket.onopen = () => {
  console.log('connected');
};
socket.onmessage = (event) => {
  const data = JSON.parse(event.data);
  console.log(data);
  longitude.value = data.lng;
  latitude.value = data.lat;
  altitude.value = data.alt;
};

</script>

<template>
  <section class="text-center">
    <h2 v-if="positionFixed" class="text-success">POSITION FIXED</h2>
    <h2 v-else class="text-danger">NO FIX</h2>
  </section>
  <hr>
  <section class="d-flex justify-content-between">
    <section>
      <b>Longitude:</b> {{longitude}}<br>
      <b>Latitude:</b> {{ latitude}}<br>
      <b>Altitude:</b> {{altitude}}<br>
    </section>
  </section>
</template>

<style scoped>

</style>
