<script setup>

import MQTTSubsection from "./subsection/MQTTSubsection.vue";

const subsections = [
  {
    id: 'mqtt',
    label: 'MQTT',
    component: MQTTSubsection
  }
];

</script>

<template>
  <div class="accordion" id="connectivityAccordion">
    <div class="accordion-item" v-for="subsection in subsections" :key="subsection.id">
      <h2 class="accordion-header">
        <button class="accordion-button collapsed" type="button" data-bs-toggle="collapse"
                :data-bs-target="`#${subsection.id}-collapse`"
                aria-expanded="false" :aria-controls="`#${subsection.id}-collapse`">
          {{ subsection.label }}
        </button>
      </h2>
      <div :id="`${subsection.id}-collapse`" class="accordion-collapse collapse"
           data-bs-parent="#connectivityAccordion">
        <Component :is="subsection.component"/>
      </div>
    </div>
  </div>
</template>

<style scoped>

</style>
