<script setup>
import {computed} from "vue";

const props = defineProps({
  connected: Boolean,
  rssi: Number
});

const BAD_RSSI = -80;
const GOOD_RSSI = -55;

const RSSI_MAPPING = {
  3: ['md-wifi', '#00c800'],
  2: ['md-wifi2bar', '#dcc800'],
  1: ['md-wifi1bar', '#c80000']
}

function getLevel(rssi) {
  if (rssi <= BAD_RSSI) {
    return 1;
  } else if (rssi >= GOOD_RSSI) {
    return 3;
  } else return 2;
}

const iconName = computed(() => {
  if (!props.connected) return 'md-wifioff';
  return RSSI_MAPPING[getLevel(props.rssi)][0];
});

const iconFill = computed(() => {
  if (!props.connected) return '#000';
  return RSSI_MAPPING[getLevel(props.rssi)][1];
});
</script>

<template>
  <Icon :name="iconName" :fill="iconFill" scale="1.5"/>
</template>

<style scoped>

</style>
