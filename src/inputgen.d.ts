/// <reference types="typescript" />

declare class SchemaFactory {
    addSchema(): SchemaBuilder;

    createMutator(type: string, json: string): MutationFactory;

    build() : boolean;

    getErrors() : Array<string>;

    delete() : void;
}

declare class SchemaBuilder {
    withName(name: string): SchemaBuilder;

    addField(): FieldBuilder;
}

declare class MutationFactory {
    getMutation() : string;
}

declare class FieldBuilder {
    withName(name: string): FieldBuilder;

    withLabel(label: FieldLabel): FieldBuilder;

    withType(type: FieldType): FieldBuilder;

    withNamedType(type: string): FieldBuilder;
}

declare enum FieldLabel {
    OPTIONAL,
    REQUIRED,
    REPEATED,
}

declare enum FieldType {
    DOUBLE,
    FLOAT,
    INT,
    FIXED,
    BOOL,
    STRING,
    BYTES,
    UINT,
    SFIXED,
    SINT
}

export default function load(): Promise<{ SchemaFactory: typeof SchemaFactory, FieldLabel: typeof FieldLabel, FieldType: typeof FieldType }>;
