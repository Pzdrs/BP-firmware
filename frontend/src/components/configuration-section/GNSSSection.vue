<script setup>
import {ref} from "vue";

const fix = ref('');
const longitude = ref(0);
const latitude = ref(0);
const altitude = ref(0);

const socket = new WebSocket(`ws://${window.location.hostname}/gnss`);
socket.onmessage = (event) => {
  const data = JSON.parse(event.data);
  longitude.value = data.data.lng;
  latitude.value = data.data.lat;
  altitude.value = data.data.alt;
  fix.value = data.data.satellites >= 4 ? '3d' : (data.data.satellites === 3 ? '2d' : 'no-fix');
};

</script>

<template>
  <section class="text-center">
    <h2 v-if="fix === '3d'" class="text-success">POSITION FIXED (3D)</h2>
    <h2 v-else-if="fix === '2d'" class="text-primary">POSITION FIXED (2D)</h2>
    <h2 v-else class="text-danger">ACQUIRING FIX</h2>
  </section>
  <hr>
  <section class="d-flex justify-content-center">
    <section>
      <b>Longitude:</b> {{ longitude }}<br>
      <b>Latitude:</b> {{ latitude }}<br>
      <b>Altitude:</b> {{ altitude }}<br>
    </section>
  </section>
</template>

<style scoped>

</style>
