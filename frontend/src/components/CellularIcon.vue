<script setup>
import {computed} from "vue";

const props = defineProps({
  connected: Boolean,
});

const iconName = computed(() => {
  if (!props.connected) return 'md-signalcellularoff';
  return 'md-signalcellularalt'
});

const iconFill = computed(() => {
  if (!props.connected) return '#000';
});
</script>

<template>
  <Icon :name="iconName" :fill="iconFill" scale="1.5"/>
</template>

<style scoped>

</style>
