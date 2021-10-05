import load from "inputgen"

const {SchemaFactory, FieldLabel, FieldType} = await load();

let factory = new SchemaFactory()
let sbuilder = factory.addSchema()
sbuilder.withName("JSONCustom")
let fbuilder = sbuilder.addField()
fbuilder.withName("hello")
fbuilder.withLabel(FieldLabel.REQUIRED)
fbuilder.withType(FieldType.STRING)
factory.build()

let mutator = factory.createMutator("JSONCustom", '{"hello":"world"}')
for (let i = 0; i < 1000; i++) {
    let p = document.createElement("p");
    p.innerText = mutator.getMutation();
    document.body.appendChild(p);
}